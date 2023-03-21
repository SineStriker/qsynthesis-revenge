#include "IWindow.h"
#include "IWindow_p.h"

#include <QDebug>

namespace Core {

    IWindowPrivate::IWindowPrivate() : q_ptr(nullptr) {
        window = nullptr;
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

    void IWindowPrivate::_q_windowClosed(QWidget *w) {
        Q_UNUSED(w);

        emit q_ptr->aboutToClose();
        deleteAllAddOns();

        emit q_ptr->closed();

        q_ptr->deleteLater();
        window = nullptr;
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
        return d_ptr->id;
    }

    QWidget *IWindow::window() const {
        return d_ptr->window;
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

    void IWindow::addWidget(const QString &id, QWidget *w) {
        if (!w) {
            qWarning() << "Core::IWindow::addWidget(): trying to add null widget";
            return;
        }
        if (d_ptr->actionItemMap.contains(id)) {
            qWarning() << "Core::IWindow::addWidget(): trying to add duplicated widget:" << id;
            return;
        }
        d_ptr->widgetMap.insert(id, w);
        emit widgetAdded(id, w);
    }

    void IWindow::removeWidget(const QString &id) {
        auto it = d_ptr->widgetMap.find(id);
        if (it == d_ptr->widgetMap.end()) {
            qWarning() << "Core::IWindow::removeWidget(): action item does not exist:" << id;
            return;
        }
        auto w = it.value();
        emit aboutToRemoveWidget(id, w);
        d_ptr->widgetMap.erase(it);
    }

    QWidget *IWindow::widget(const QString &id) const {
        auto it = d_ptr->widgetMap.find(id);
        if (it != d_ptr->widgetMap.end()) {
            return it.value();
        }
        return nullptr;
    }

    QWidgetList IWindow::widgets() const {
        return d_ptr->widgetMap.values();
    }

    void IWindow::addObject(QObject *obj) {
        addObject({}, obj);
    }

    void IWindow::addObject(const QString &id, QObject *obj) {
        if (!obj) {
            qWarning() << "Core::IWindow::addObject(): trying to add null object";
            return;
        }

        QWriteLocker locker(&d_ptr->objectListLock);
        auto &set = d_ptr->objectMap[id];
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
        QWriteLocker locker(&d_ptr->objectListLock);

        auto it = d_ptr->objectIndexes.find(obj);
        if (it == d_ptr->objectIndexes.end()) {
            qWarning() << "Core::IWindow::removeObject(): obj does not exist:" << obj;
            return;
        }
        QString id = it.value();
        auto it2 = d_ptr->objectMap.find(id);
        if (it2 != d_ptr->objectMap.end()) {
            auto &set = it2.value();

            emit aboutToRemoveObject(id, obj);
            set.remove(obj);

            d_ptr->objectIndexes.remove(obj);
            if (set.isEmpty()) {
                d_ptr->objectMap.erase(it2);
            }
        }
    }

    void IWindow::removeObjects(const QString &id) {
        QWriteLocker locker(&d_ptr->objectListLock);

        auto it2 = d_ptr->objectMap.find(id);
        if (it2 != d_ptr->objectMap.end()) {
            auto &set = it2.value();

            for (const auto &obj : qAsConst(set)) {
                emit aboutToRemoveObject(id, obj);
                d_ptr->objectIndexes.remove(obj);
            }

            d_ptr->objectMap.erase(it2);
        }
    }

    QList<QObject *> IWindow::getObjects(const QString &id) const {
        QReadLocker locker(&d_ptr->objectListLock);

        auto it2 = d_ptr->objectMap.find(id);
        if (it2 != d_ptr->objectMap.end()) {
            return it2->values();
        }

        return {};
    }

    QList<QObject *> IWindow::allObjects() const {
        QReadLocker locker(&d_ptr->objectListLock);
        return d_ptr->objectIndexes.keys();
    }

    QReadWriteLock *IWindow::objectListLock() const {
        return &d_ptr->objectListLock;
    }

    void IWindow::addActionItem(const QString &id, ActionItem *item) {
        if (!item) {
            qWarning() << "Core::IWindow::addActionItem(): trying to add null action item";
            return;
        }
        if (d_ptr->actionItemMap.contains(id)) {
            qWarning() << "Core::IWindow::addActionItem(): trying to add duplicated action item:" << id;
            return;
        }
        d_ptr->actionItemMap.insert(id, item);
    }

    void IWindow::removeActionItem(const QString &id) {
        auto it = d_ptr->actionItemMap.find(id);
        if (it == d_ptr->actionItemMap.end()) {
            qWarning() << "Core::IWindow::addActionItem(): action item does not exist:" << id;
            return;
        }
        d_ptr->actionItemMap.erase(it);
    }

    ActionItem *IWindow::actionItem(const QString &id) const {
        auto it = d_ptr->actionItemMap.find(id);
        if (it != d_ptr->actionItemMap.end()) {
            return it.value();
        }
        return nullptr;
    }

    QList<ActionItem *> IWindow::actionItems() const {
        return d_ptr->actionItemMap.values();
    }

    void IWindow::reloadActions() {
    }

    IWindow::IWindow(const QString &id, QObject *parent) : IWindow(*new IWindowPrivate(), id, parent) {
    }

    IWindow::~IWindow() {
        // Remove action items
        for (auto &item : qAsConst(d_ptr->actionItemMap)) {
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