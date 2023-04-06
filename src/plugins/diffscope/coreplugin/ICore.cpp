#include "ICore.h"
#include "ICore_p.h"

#include "QMConsole.h"

#include <QApplication>
#include <QMessageBox>

#include "Internal/Dialogs/SettingsDialog.h"

namespace Core {

    ICorePrivate::ICorePrivate() {
        actionSystem = nullptr;
        windowSystem = nullptr;
        dialogHelper = nullptr;
    }

    void ICorePrivate::init() {
        Q_Q(ICore);

        qApp->setQuitOnLastWindowClosed(false);

        actionSystem = new ActionSystem(q);
        windowSystem = new WindowSystem(q);
        dialogHelper = new DialogHelper(q);

        settingCatalog = new SettingCatalog(q);
    }

    static ICore *m_instance = nullptr;

    ICore *ICore::instance() {
        return m_instance;
    }

    QString ICore::mainTitle() {
        return QString("%1 %2").arg(QApplication::organizationName(), QApplication::applicationName());
    }

    QString ICore::displayTitle(const QString &text) {
        return QString("%1 - %2").arg(text, mainTitle());
    }

    void ICore::aboutApp(QWidget *parent) {
        QString title = tr("About %1").arg(qAppName());
        QString text = tr("%1 %2, Copyright OpenVPI.").arg(mainTitle(), QApplication::applicationVersion());
#ifdef Q_OS_WINDOWS
        QMConsole::instance()->MsgBox(parent, QMConsole::Information, title, text);
#else
        QMessageBox::information(parent, title, text);
#endif
    }

    int ICore::showSettingsDialog(const QString &id, QWidget *parent) {
        static Internal::SettingsDialog *dlg = nullptr;

        if (dlg) {
            dlg->selectPage(id);
            return 0;
        }

        int code;
        {
            Internal::SettingsDialog dlg2(parent);
            dlg = &dlg2;
            dlg2.selectPage(id);
            code = dlg2.exec();
            dlg = nullptr;
        }

        return code;
    }

    ActionSystem *ICore::actionSystem() const {
        Q_D(const ICore);
        return d->actionSystem;
    }

    WindowSystem *ICore::windowSystem() const {
        Q_D(const ICore);
        return d->windowSystem;
    }

    DialogHelper *ICore::dialogHelper() const {
        Q_D(const ICore);
        return d->dialogHelper;
    }

    SettingCatalog *ICore::settingCatalog() const {
        Q_D(const ICore);
        return d->settingCatalog;
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