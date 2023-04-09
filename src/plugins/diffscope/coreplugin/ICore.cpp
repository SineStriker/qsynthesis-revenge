#include "ICore.h"
#include "ICore_p.h"

#include "QMConsole.h"

#include <QApplication>
#include <QMessageBox>

#include "Internal/Dialogs/SettingsDialog.h"
#include "Internal/Dialogs/WizardDialog.h"

namespace Core {

    ICorePrivate::ICorePrivate() {
    }

    void ICorePrivate::init() {
        Q_Q(ICore);
        preferenceRegistry = new PreferenceRegistry(q);
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
            return -1;
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

    int ICore::showWizardDialog(const QString &id, QWidget *parent) {
        static Internal::WizardDialog *dlg = nullptr;

        if (dlg) {
            return -1;
        }

        int code;
        {
            Internal::WizardDialog dlg2(parent);
            dlg = &dlg2;
            // dlg2.selectPage(id);
            code = dlg2.exec();
            dlg = nullptr;
        }

        return code;
    }

    PreferenceRegistry *ICore::preferenceRegistry() const {
        Q_D(const ICore);
        return d->preferenceRegistry;
    }

    ICore::ICore(QObject *parent) : ICore(*new ICorePrivate(), parent) {
    }

    ICore::~ICore() {
        m_instance = nullptr;
    }

    ICore::ICore(ICorePrivate &d, QObject *parent) : ICoreBase(d, parent) {
        m_instance = this;

        d.init();
    }

}