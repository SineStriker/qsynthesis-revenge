#include "ICore.h"
#include "ICore_p.h"

#include "CoreApi/ActionSystem.h"
#include "CoreApi/WindowSystem.h"

#include "QMConsole.h"

#include <QApplication>

namespace Core {

    ICorePrivate::ICorePrivate() : q_ptr(nullptr) {
        actionSystem = nullptr;
        windowSystem = nullptr;
    }

    void ICorePrivate::init() {
        qApp->setQuitOnLastWindowClosed(false);

        actionSystem = new ActionSystem(q_ptr);
        windowSystem = new WindowSystem(q_ptr);
    }

    static ICore *m_instance = nullptr;

    ICore *ICore::instance() {
        return m_instance;
    }

    void ICore::aboutApp(QWidget *parent) {
        QString title = tr("About %1").arg(qAppName());
        QString text = tr("%1 %2, Copyright OpenVPI.").arg(qAppName(), QApplication::applicationVersion());
#ifdef Q_OS_WINDOWS
        QMConsole::instance()->MsgBox(parent, QMConsole::Information, title, text);
#else
        QMessageBox::information(parent, title, text);
#endif
    }

    ActionSystem *ICore::actionSystem() const {
        return d_ptr->actionSystem;
    }

    WindowSystem *ICore::windowSystem() const {
        return d_ptr->windowSystem;
    }

    ICore::ICore(QObject *parent) : ICore(*new ICorePrivate(), parent) {
    }

    ICore::~ICore() {
        m_instance = nullptr;
    }

    ICore::ICore(ICorePrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
        m_instance = this;

        d.q_ptr = this;
        d.init();
    }
}