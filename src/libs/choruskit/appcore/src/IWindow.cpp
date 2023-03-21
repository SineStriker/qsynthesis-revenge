#include "IWindow.h"
#include "IWindow_p.h"

#include "IWindowAddOn_p.h"
#include "WindowCloseFilter_p.h"
#include "WindowSystem_p.h"

#include <QDebug>
#include <QEvent>

namespace Core {

    IWindowPrivate::IWindowPrivate() : q_ptr(nullptr) {
        window = nullptr;
        autoCorrectTitle = false;
        isCorrectingTitle = false;
    }

    IWindowPrivate::~IWindowPrivate() {
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
    }

    void IWindowPrivate::deleteAllAddOns() {
        for (auto it2 = addOns.rbegin(); it2 != addOns.rend(); ++it2) {
            auto &addOn = *it2;
            // Call 1
            delete addOn;
        }
    }

    bool IWindowPrivate::eventFilter(QObject *obj, QEvent *event) {
        Q_Q(IWindow);
        if (obj == window) {
            switch (event->type()) {
                case QEvent::WindowTitleChange:
                    if (!isCorrectingTitle) {
                        isCorrectingTitle = true;
                        window->setWindowTitle(q->correctWindowTitle(window->windowTitle()));
                    } else {
                        isCorrectingTitle = false;
                    }
                    break;
                default:
                    break;
            }
        }
        return QObject::eventFilter(obj, event);
    }

    void IWindowPrivate::_q_windowClosed(QWidget *w) {
        Q_Q(IWindow);

        Q_UNUSED(w);

        emit q->aboutToClose();
        deleteAllAddOns();

        emit q->closed();

        q->deleteLater();
        window = nullptr;
    }

    void IWindowPrivate::setWindow(QWidget *w, WindowSystemPrivate *d) {
        Q_Q(IWindow);

        window = w;

        // Filter window title event
        w->installEventFilter(this);

        auto filter = new WindowCloseFilter(window);
        connect(filter, &WindowCloseFilter::windowClosed, this, &IWindowPrivate::_q_windowClosed);

        // Setup window
        q->setupWindow();

        // Call all add-ons
        for (auto fac : qAsConst(d->addOnFactories)) {
            if (!fac->predicate(q)) {
                continue;
            }
            auto addOn = fac->create(q);
            addOn->d_ptr->iWin = q;
            addOns.push_back(addOn);
        }

        initAllAddOns();
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

    QWidget *IWindow::window() const {
        Q_D(const IWindow);
        return d->window;
    }

    QMenuBar *IWindow::menuBar() const {
        return nullptr;
    }

    void IWindow::setMenuBar(QMenuBar *menuBar) {
        Q_UNUSED(menuBar);
    }

    QWidget *IWindow::centralWidget() const {
        return nullptr;
    }

    void IWindow::setCentralWidget(QWidget *widget) {
        Q_UNUSED(widget);
    }

    QStatusBar *IWindow::statusBar() const {
        return nullptr;
    }

    void IWindow::setStatusBar(QStatusBar *statusBar) {
        Q_UNUSED(statusBar);
    }

    bool IWindow::windowTitleCorrectionEnabled() const {
        Q_D(const IWindow);
        return d->autoCorrectTitle;
    }

    void IWindow::setWindowTitleCorrectionEnabled(bool enabled) {
        Q_D(IWindow);
        d->autoCorrectTitle = enabled;
    }

    QString IWindow::correctWindowTitle(const QString &title) const {
        return title;
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

    void IWindow::addObject(QObject *obj) {
        addObject({}, obj);
    }

    void IWindow::addObject(const QString &id, QObject *obj) {
        Q_D(IWindow);
        if (!obj) {
            qWarning() << "Core::IWindow::addObject(): trying to add null object";
            return;
        }

        QWriteLocker locker(&d->objectListLock);
        auto &set = d->objectMap[id];
        if (set.contains(obj)) {
            qWarning() << "Core::IWindow::addObject(): trying to add duplicated object:" << id << obj;
            return;
        }
        set.insert(obj);
        emit objectAdded(id, obj);
    }

    void IWindow::addObjects(const QString &id, const QList<QObject *> &objs) {
        for (const auto &obj : objs) {
            addObject(id, obj);
        }
    }

    void IWindow::removeObject(QObject *obj) {
        Q_D(IWindow);
        QWriteLocker locker(&d->objectListLock);

        auto it = d->objectIndexes.find(obj);
        if (it == d->objectIndexes.end()) {
            qWarning() << "Core::IWindow::removeObject(): obj does not exist:" << obj;
            return;
        }
        QString id = it.value();
        auto it2 = d->objectMap.find(id);
        if (it2 != d->objectMap.end()) {
            auto &set = it2.value();

            emit aboutToRemoveObject(id, obj);
            set.remove(obj);

            d->objectIndexes.remove(obj);
            if (set.isEmpty()) {
                d->objectMap.erase(it2);
            }
        }
    }

    void IWindow::removeObjects(const QString &id) {
        Q_D(IWindow);
        QWriteLocker locker(&d->objectListLock);

        auto it2 = d->objectMap.find(id);
        if (it2 != d->objectMap.end()) {
            auto &set = it2.value();

            for (const auto &obj : qAsConst(set)) {
                emit aboutToRemoveObject(id, obj);
                d->objectIndexes.remove(obj);
            }

            d->objectMap.erase(it2);
        }
    }

    QList<QObject *> IWindow::getObjects(const QString &id) const {
        Q_D(const IWindow);
        QReadLocker locker(&d->objectListLock);

        auto it2 = d->objectMap.find(id);
        if (it2 != d->objectMap.end()) {
            return it2->values();
        }

        return {};
    }

    QList<QObject *> IWindow::allObjects() const {
        Q_D(const IWindow);
        QReadLocker locker(&d->objectListLock);
        return d->objectIndexes.keys();
    }

    QReadWriteLock *IWindow::objectListLock() const {
        Q_D(const IWindow);
        return &d->objectListLock;
    }

    void IWindow::addActionItem(const QString &id, ActionItem *item) {
        Q_D(IWindow);
        if (!item) {
            qWarning() << "Core::IWindow::addActionItem(): trying to add null action item";
            return;
        }
        if (d->actionItemMap.contains(id)) {
            qWarning() << "Core::IWindow::addActionItem(): trying to add duplicated action item:" << id;
            return;
        }
        d->actionItemMap.insert(id, item);
    }

    void IWindow::removeActionItem(const QString &id) {
        Q_D(IWindow);
        auto it = d->actionItemMap.find(id);
        if (it == d->actionItemMap.end()) {
            qWarning() << "Core::IWindow::addActionItem(): action item does not exist:" << id;
            return;
        }
        d->actionItemMap.erase(it);
    }

    ActionItem *IWindow::actionItem(const QString &id) const {
        Q_D(const IWindow);
        auto it = d->actionItemMap.find(id);
        if (it != d->actionItemMap.end()) {
            return it.value();
        }
        return nullptr;
    }

    QList<ActionItem *> IWindow::actionItems() const {
        Q_D(const IWindow);
        return d->actionItemMap.values();
    }

    void IWindow::reloadActions() {
    }

    IWindow::IWindow(const QString &id, QObject *parent) : IWindow(*new IWindowPrivate(), id, parent) {
    }

    IWindow::~IWindow() {
        Q_D(IWindow);

        // Remove action items
        for (auto &item : qAsConst(d->actionItemMap)) {
            delete item;
        }
    }

    void IWindow::setupWindow() {
        // Do nothing
    }

    IWindow::IWindow(IWindowPrivate &d, const QString &id, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();

        d.id = id;
    }

}