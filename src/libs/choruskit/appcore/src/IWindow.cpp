#include "IWindow.h"
#include "IWindow_p.h"

#include "IWindowAddOn_p.h"
#include "WindowCloseFilter_p.h"
#include "WindowSystem_p.h"

#include <QDebug>
#include <QEvent>

#include <private/qwidget_p.h>

static const int DELAYED_INITIALIZE_INTERVAL = 5; // ms

namespace Core {

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
        m_closed = false;
        m_shortcutsDirty = false;
        actionFilter = new WindowActionFilter(this);
        connect(actionFilter, &WindowActionFilter::actionChanged, this, &IWindowPrivate::_q_actionChanged);
    }

    IWindowPrivate::~IWindowPrivate() {
        tryStopDelayedTimer();
    }

    void IWindowPrivate::init() {
    }

    void IWindowPrivate::initAllAddOns() {
        for (auto &addOn : qAsConst(addOns)) {
            // Call 1
            addOn->initialize();
        }

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
    }

    void IWindowPrivate::deleteAllAddOns() {
        for (auto it2 = addOns.rbegin(); it2 != addOns.rend(); ++it2) {
            auto &addOn = *it2;
            // Call 1
            delete addOn;
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

        tryStopDelayedTimer();

        m_closed = true;

        q->windowAboutToClose();

        emit q->aboutToClose();

        deleteAllAddOns();

        q->windowClosed();

        emit q->closed();

        q->setWindow(nullptr);
        delete q;
    }

    void IWindowPrivate::_q_actionChanged(QWidget *w) {
        Q_UNUSED(w);

        m_shortcutsDirty = true;
    }

    void IWindowPrivate::_q_actionWidgetDestroyed(QObject *obj) {
        shortcutContextWidgets.remove(static_cast<QWidget *>(obj));
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

    void IWindowPrivate::setWindow(QWidget *w, WindowSystemPrivate *d) {
        Q_Q(IWindow);

        q->setWindow(w);

        auto filter = new WindowCloseFilter(this, q->window());
        connect(filter, &WindowCloseFilter::windowClosed, this, &IWindowPrivate::_q_windowClosed);

        // Setup window
        q->setupWindow();

        // Call all add-ons
        for (auto &fac : qAsConst(d->addOnFactories)) {
            if (!fac->predicate(q)) {
                continue;
            }

            auto addOn = fac->create(q);
            if (!addOn) {
                qWarning() << "Core::WindowSystem::createWindow(): window add-on factory creates null instance:"
                           << typeid(*fac).name();
                continue;
            }

            addOn->d_ptr->iWin = q;
            addOns.push_back(addOn);
        }

        initAllAddOns();

        // Add-ons finished
        q->windowAddOnsFinished();
    }

    IWindowFactory::IWindowFactory(const QString &id, AvailableCreator creator) : d_ptr(new IWindowFactoryPrivate()) {
        d_ptr->id = id;
        d_ptr->creator = creator;
    }

    IWindowFactory::~IWindowFactory() {
    }

    QString IWindowFactory::id() const {
        return d_ptr->id;
    }

    IWindowFactory::AvailableCreator IWindowFactory::creator() const {
        return d_ptr->creator;
    }

    IWindow *IWindowFactory::create(QObject *parent) {
        return nullptr;
    }

    IWindow *IWindowFactory::create(const QString &id, QObject *parent) {
        return nullptr;
    }

    QString IWindow::id() const {
        Q_D(const IWindow);
        return d->id;
    }

    void IWindow::addWidget(const QString &id, QWidget *w) {
        Q_D(IWindow);
        if (!w) {
            qWarning() << "Core::IWindow::addWidget(): trying to add null widget";
            return;
        }
        if (d->actionItemMap.contains(id)) {
            qWarning() << "Core::IWindow::addWidget(): trying to add duplicated widget:" << id;
            return;
        }
        d->widgetMap.insert(id, w);
        emit widgetAdded(id, w);
    }

    void IWindow::removeWidget(const QString &id) {
        Q_D(IWindow);
        auto it = d->widgetMap.find(id);
        if (it == d->widgetMap.end()) {
            qWarning() << "Core::IWindow::removeWidget(): action item does not exist:" << id;
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
            qWarning() << "Core::IWindow::addActionItem(): trying to add null action item";
            return;
        }

        if (!item->spec()) {
            qWarning() << "Core::IWindow::addActionItem(): trying to add unidentified item" << item->id();
            return;
        }

        if (d->actionItemMap.contains(item->id())) {
            qWarning() << "Core::IWindow::addActionItem(): trying to add duplicated action item:" << item->id();
            return;
        }
        d->actionItemMap.insert(item->id(), item);
    }

    void IWindow::addActionItems(const QList<Core::ActionItem *> &items) {
        for (const auto &item : items) {
            addActionItem(item);
        }
    }

    void IWindow::removeActionItem(Core::ActionItem *item) {
        if (item == nullptr) {
            qWarning() << "Core::IWindow::removeActionItem(): trying to remove null item";
            return;
        }
        removeActionItem(item->id());
    }

    void IWindow::removeActionItem(const QString &id) {
        Q_D(IWindow);
        auto it = d->actionItemMap.find(id);
        if (it == d->actionItemMap.end()) {
            qWarning() << "Core::IWindow::removeActionItem(): action item does not exist:" << id;
            return;
        }
        d->actionItemMap.erase(it);
    }

    ActionItem *IWindow::actionItem(const QString &id) const {
        Q_D(const IWindow);
        return d->actionItemMap.value(id, nullptr);
    }

    QList<ActionItem *> IWindow::actionItems() const {
        Q_D(const IWindow);
        return d->actionItemMap.values();
    }

    void IWindow::addShortcutContext(QWidget *w) {
        Q_D(IWindow);
        if (!w) {
            qWarning() << "Core::IWindow::addShortcutContext(): trying to add null widget";
            return;
        }

        if (d->shortcutContextWidgets.contains(w)) {
            qWarning() << "Core::IWindow::addShortcutContext(): trying to add duplicated widget:" << w;
            return;
        }

        d->shortcutContextWidgets.append(w);
        connect(w, &QObject::destroyed, d, &IWindowPrivate::_q_actionWidgetDestroyed);
        w->installEventFilter(d->actionFilter);

        if (static_cast<QWidgetHacker *>(w)->actionCount() > 0)
            d->m_shortcutsDirty = true;
    }

    void IWindow::removeShortcutContext(QWidget *w) {
        Q_D(IWindow);
        auto it = d->shortcutContextWidgets.find(w);
        if (it == d->shortcutContextWidgets.end()) {
            qWarning() << "Core::IWindow::removeShortcutContext(): widget does not exist:" << w;
            return;
        }

        w->removeEventFilter(d->actionFilter);
        disconnect(w, &QObject::destroyed, d, &IWindowPrivate::_q_actionWidgetDestroyed);
        d->shortcutContextWidgets.erase(it);

        if (static_cast<QWidgetHacker *>(w)->actionCount() > 0)
            d->m_shortcutsDirty = true;
    }

    QList<QWidget *> IWindow::shortcutContexts() const {
        Q_D(const IWindow);
        return d->shortcutContextWidgets.values();
    }

    bool IWindow::hasShortcut(const QKeySequence &key) const {
        Q_D(const IWindow);
        if (d->m_shortcutsDirty) {

            QList<QWidget *> menus;
            decltype(d->shortcutsMap) map;

            auto goThroughMenu = [&](QWidget *w) {
                for (QAction *action : w->actions()) {
                    auto menu = action->menu();
                    if (menu) {
                        menus.append(menu);
                    }
                    for (const auto &sh : action->shortcuts())
                        map.insert(sh);
                }
            };

            menus.reserve(d->shortcutContextWidgets.size());
            menus << d->shortcutContextWidgets.values();

            while (!menus.isEmpty()) {
                goThroughMenu(menus.takeFirst());
            }

            d->shortcutsMap = std::move(map);
            d->m_shortcutsDirty = false;
        }

        return d->shortcutsMap.contains(key);
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

    void IWindow::windowAddOnsFinished() {
        // Do nothing
    }

    void IWindow::windowAboutToClose() {
        // Do nothing
    }

    void IWindow::windowClosed() {
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

}