#include "ICore.h"

#include <CMenu.h>
#include <CoreApi/ICoreBase_p.h>
#include <QMConsole.h>

#include <QApplication>
#include <QChildEvent>
#include <QMessageBox>

#include <extensionsystem/pluginmanager.h>

#include "Internal/Dialogs/SettingsDialog.h"

namespace Core {

    class ICorePrivate : ICoreBasePrivate {
        Q_DECLARE_PUBLIC(ICore)
    public:
        ICorePrivate() {
        }

        void init() {
        }
    };

    ICore *ICore::instance() {
        return qobject_cast<ICore *>(ICoreBase::instance());
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

    class PopUpMenuWatcher : public QObject {
    public:
        explicit PopUpMenuWatcher(QWidget *parent = nullptr) : QObject(parent) {
            parent->installEventFilter(this);
        }

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override {
            if (obj == parent()) {
                switch (event->type()) {
                    case QEvent::ChildPolished: {
                        auto e = static_cast<QChildEvent *>(event);
                        auto child = e->child();
                        if (child->inherits("QMenu") && child->property("core-style").isNull()) {
                            auto menu = qobject_cast<QMenu *>(child);
                            menu->setProperty("core-style", true);
                            menu->style()->polish(menu);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
            return QObject::eventFilter(obj, event);
        }
    };

    void ICore::autoPolishPopupMenu(QWidget *w) {
        new PopUpMenuWatcher(w);
    }

    QMenu *ICore::createCoreMenu(QWidget *parent) {
        auto menu = new CMenu(parent);
        menu->setProperty("core-style", true);
        return menu;
    }

    ICore::ICore(QObject *parent) : ICore(*new ICorePrivate(), parent) {
    }

    ICore::~ICore() {
    }

    ICore::ICore(ICorePrivate &d, QObject *parent) : ICoreBase(d, parent) {
        d.init();
    }


}