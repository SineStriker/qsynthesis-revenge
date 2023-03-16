#include "IWindow.h"
#include "IWindow_p.h"

#include <QDebug>

namespace Core {

    IWindowPrivate::IWindowPrivate() : IWindowPrivate(IWindow::Home) {
    }

    IWindowPrivate::IWindowPrivate(IWindow::Type type) : type(type), q_ptr(nullptr) {
    }

    void IWindowPrivate::init() {
    }

    IWindow::Type IWindow::type() const {
        return d_ptr->type;
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

    IWindow::IWindow(QObject *parent) : IWindow(*new IWindowPrivate(), parent) {
    }

    IWindow::~IWindow() {
    }

    IWindow::IWindow(IWindowPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.init();
    }

}