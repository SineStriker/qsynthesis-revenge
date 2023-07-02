#include "IWindow.h"
#include "IWindow_p.h"

#include "ICoreBase.h"
#include "IWindowAddOn_p.h"
#include "ShortcutContext_p.h"
#include "WindowCloseFilter_p.h"
#include "WindowSystem_p.h"

#include <QDebug>
#include <QEvent>

#include <private/qwidget_p.h>

static const int DELAYED_INITIALIZE_INTERVAL = 5; // ms

namespace Core {

#define myWarning(func) (qWarning().nospace() << "Core::IWindow::" << (func) << "():").space()

    class QWidgetHacker : public QWidget {
    public:
        int actionCount() const {
            auto d = static_cast<QWidgetPrivate *>(d_ptr.data());
            return d->actions.count();
        }

        friend class IWindow;
        friend class IWindowPrivate;
    };

    IWindowPrivate::IWindowPrivate() {
        Q_Q(IWindow);

        m_closed = false;
    }

    IWindowPrivate::~IWindowPrivate() {
        tryStopDelayedTimer();
    }

    void IWindowPrivate::init() {
    }

    void IWindowPrivate::setWindow(QWidget *w, WindowSystemPrivate *d, const std::function<void(IWindow *)> &pre) {
        Q_Q(IWindow);

        q->setWindow(w);

        shortcutCtx = new QMShortcutContext(this);

        closeFilter = new WindowCloseFilter(this, q->window());
        connect(closeFilter, &WindowCloseFilter::windowClosed, this, &IWindowPrivate::_q_windowClosed);

        if (pre)
            pre(q);

        // Setup window
        q->setupWindow();

        // Call all add-ons
        for (auto &fac : qAsConst(d->addOnFactories)) {
            if (!fac->predicate(q)) {
                continue;
            }

            auto addOn = fac->create(q);
            if (!addOn) {
                myWarning(__func__) << "window add-on factory creates null instance:" << typeid(*fac).name();
                continue;
            }

            addOn->d_ptr->iWin = q;
            addOns.push_back(addOn);
        }

        // Initialize
        for (auto &addOn : qAsConst(addOns)) {
            // Call 1
            addOn->initialize();
        }

        q->windowAddOnsInitialized();

        // ExtensionsInitialized
        for (auto it2 = addOns.rbegin(); it2 != addOns.rend(); ++it2) {
            auto &addOn = *it2;
            // Call 2
            addOn->extensionsInitialized();
        }

        // Delayed initialize
        delayedInitializeQueue = addOns;

        delayedInitializeTimer = new QTimer();
        delayedInitializeTimer->setInterval(DELAYED_INITIALIZE_INTERVAL);
        delayedInitializeTimer->setSingleShot(true);
        connect(delayedInitializeTimer, &QTimer::timeout, this, &IWindowPrivate::nextDelayedInitialize);
        delayedInitializeTimer->start();

        // Add-ons finished
        q->windowAddOnsFinished();
    }

    void IWindowPrivate::closeWindow() {
        Q_Q(IWindow);
        auto w = q->window();

        m_closed = true;

        if (!w->isHidden())
            w->hide();

        delete shortcutCtx;
        shortcutCtx = nullptr;
    }

    void IWindowPrivate::deleteAllAddOns() {
        for (auto it2 = addOns.rbegin(); it2 != addOns.rend(); ++it2) {
            auto &addOn = *it2;
            // Call 1
            delete addOn;
        }
    }

    void IWindowPrivate::tryStopDelayedTimer() {
        // Stop delayed initializations
        if (delayedInitializeTimer) {
            if (delayedInitializeTimer->isActive()) {
                delayedInitializeTimer->stop();
            }
            delete delayedInitializeTimer;
            delayedInitializeTimer = nullptr;
        }
    }

    void IWindowPrivate::nextDelayedInitialize() {
        Q_Q(IWindow);

        while (!delayedInitializeQueue.empty()) {
            auto addOn = delayedInitializeQueue.front();
            delayedInitializeQueue.pop_front();

            bool delay = addOn->delayedInitialize();
            if (delay)
                break; // do next delayedInitialize after a delay
        }
        if (delayedInitializeQueue.empty()) {
            delete delayedInitializeTimer;
            delayedInitializeTimer = nullptr;
            emit q->initializationDone();
        } else {
            delayedInitializeTimer->start();
        }
    }

    void IWindowPrivate::_q_windowClosed(QWidget *w) {
        Q_Q(IWindow);

        Q_UNUSED(w);

        tryStopDelayedTimer();

        closeWindow();

        q->windowAboutToClose();

        emit q->aboutToClose();

        deleteAllAddOns();

        q->windowClosed();

        ICoreBase::instance()->windowSystem()->d_func()->windowClosed(q);
        emit q->closed();

        q->setWindow(nullptr);
        delete q;
    }

    IWindowFactory::IWindowFactory(const QString &id) : d_ptr(new IWindowFactoryPrivate()) {
        d_ptr->id = id;
    }

    IWindowFactory::~IWindowFactory() {
    }

    QString IWindowFactory::id() const {
        return d_ptr->id;
    }

    IWindow *IWindowFactory::create(QObject *parent) {
        return nullptr;
    }

    QString IWindow::id() const {
        Q_D(const IWindow);
        return d->id;
    }

    void IWindow::addWidget(const QString &id, QWidget *w) {
        Q_D(IWindow);
        if (!w) {
            myWarning(__func__) << "trying to add null widget";
            return;
        }
        if (d->actionItemMap.contains(id)) {
            myWarning(__func__) << "trying to add duplicated widget:" << id;
            return;
        }
        d->widgetMap.insert(id, w);
        emit widgetAdded(id, w);
    }

    void IWindow::removeWidget(const QString &id) {
        Q_D(IWindow);
        auto it = d->widgetMap.find(id);
        if (it == d->widgetMap.end()) {
            myWarning(__func__) << "action item does not exist:" << id;
            return;
        }
        auto w = it.value();
        emit aboutToRemoveWidget(id, w);
        d->widgetMap.erase(it);
    }

    QWidget *IWindow::widget(const QString &id) const {
        Q_D(const IWindow);
        auto it = d->widgetMap.find(id);
        if (it != d->widgetMap.end()) {
            return it.value();
        }
        return nullptr;
    }

    QWidgetList IWindow::widgets() const {
        Q_D(const IWindow);
        return d->widgetMap.values();
    }

    void IWindow::addActionItem(ActionItem *item) {
        Q_D(IWindow);
        if (!item) {
            myWarning(__func__) << "trying to add null action item";
            return;
        }

        if (!item->spec()) {
            myWarning(__func__) << "trying to add unidentified item" << item->id();
            return;
        }

        if (d->actionItemMap.contains(item->id())) {
            myWarning(__func__) << "trying to add duplicated action item:" << item->id();
            return;
        }
        d->actionItemMap.append(item->id(), item);

        actionItemAdded(item);
    }

    void IWindow::addActionItems(const QList<Core::ActionItem *> &items) {
        for (const auto &item : items) {
            addActionItem(item);
        }
    }

    void IWindow::removeActionItem(Core::ActionItem *item) {
        if (item == nullptr) {
            myWarning(__func__) << "trying to remove null item";
            return;
        }
        removeActionItem(item->id());
    }

    void IWindow::removeActionItem(const QString &id) {
        Q_D(IWindow);
        auto it = d->actionItemMap.find(id);
        if (it == d->actionItemMap.end()) {
            myWarning(__func__) << "action item does not exist:" << id;
            return;
        }
        auto item = it.value();
        d->actionItemMap.erase(it);

        actionItemRemoved(item);
    }

    ActionItem *IWindow::actionItem(const QString &id) const {
        Q_D(const IWindow);
        return d->actionItemMap.value(id, nullptr);
    }

    QList<ActionItem *> IWindow::actionItems() const {
        Q_D(const IWindow);
        return d->actionItemMap.values();
    }

    void IWindow::addShortcutContext(QWidget *w, ShortcutContextPriority priority) {
        Q_D(IWindow);
        d->shortcutCtx->addWidget(w, priority);
    }

    void IWindow::removeShortcutContext(QWidget *w) {
        Q_D(IWindow);
        d->shortcutCtx->removeWidget(w);
    }

    QList<QWidget *> IWindow::shortcutContexts() const {
        Q_D(const IWindow);
        return d->shortcutCtx->widgets();
    }

    bool IWindow::hasDragFileHandler(const QString &suffix) {
        Q_D(const IWindow);
        if (suffix.isEmpty())
            return false;

        return d->dragFileHandlerMap.contains(suffix.toLower());
    }

    void IWindow::setDragFileHandler(const QString &suffix, QObject *obj, const char *member, int maxCount) {
        Q_D(IWindow);

        if (suffix.isEmpty())
            return;

        if (!obj || maxCount < 0) {
            removeDragFileHandler(suffix);
            return;
        }
        d->dragFileHandlerMap[suffix.toLower()] = {obj, member, maxCount};
    }

    void IWindow::removeDragFileHandler(const QString &suffix) {
        Q_D(IWindow);
        if (suffix.isEmpty())
            return;

        d->dragFileHandlerMap.remove(suffix.toLower());
    }

    IWindow::IWindow(const QString &id, QObject *parent) : IWindow(*new IWindowPrivate(), id, parent) {
    }

    IWindow::~IWindow() {
        Q_D(IWindow);
    }

    void IWindow::setupWindow() {
        // Do nothing
    }

    void IWindow::windowAddOnsInitialized() {
        // Do nothing
    }

    void IWindow::windowAddOnsFinished() {
        // Do nothing
    }

    void IWindow::windowAddOnsBroadcast(const QString &msg, const QVariantHash &attributes) {
        Q_D(const IWindow);
        for (const auto &item : d->addOns) {
            item->broadcast(msg, attributes);
        }
    }

    void IWindow::windowAboutToClose() {
        // Do nothing
    }

    void IWindow::windowClosed() {
        // Do nothing
    }

    void IWindow::actionItemAdded(ActionItem *item) {
        // Do nothing
    }

    void IWindow::actionItemRemoved(ActionItem *item) {
        // Do nothing
    }

    IWindow::IWindow(IWindowPrivate &d, const QString &id, QObject *parent) : ObjectPool(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

    bool IWindow::isEffectivelyClosed() const {
        Q_D(const IWindow);
        return d->m_closed;
    }

} // namespace Core
