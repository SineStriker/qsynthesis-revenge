#include "IWindow.h"
#include "IWindow_p.h"

#include <QDebug>

namespace Core {

    IWindowPrivate::IWindowPrivate() : q_ptr(nullptr) {
        window = nullptr;
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

    IWindow::IWindow(const QString &id, QObject *parent) : IWindow(*new IWindowPrivate(), id, parent) {
    }

    IWindow::~IWindow() {
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

    void IWindow::addActionItem(const QString &id, ActionItem *item) {
        if (!item) {
            qWarning() << "Core::IWindow::addActionItem(): trying to add null action item";
            return;
        }
        if (d_ptr->actionItemMap.contains(id)) {
            qWarning() << "Core::IWindow::addActionItem(): trying to add duplicated action item";
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

        d_ptr->actionItemMap.remove(id);
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

    IWindow::IWindow(IWindowPrivate &d, const QString &id, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();

        d.id = id;
    }

}