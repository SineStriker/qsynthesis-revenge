#include "ICore.h"

#include <csignal>

#include <QApplication>
#include <QChildEvent>
#include <QMessageBox>
#include <QScreen>

#include <CMenu.h>
#include <QMConsole.h>
#include <QMSystem.h>
#include <QMView.h>

#include <CoreApi/ICoreBase_p.h>

#include <extensionsystem/pluginmanager.h>

#include "Internal/Dialogs/SettingsDialog.h"
#include "Window/IHomeWindow.h"

#include "ChorusKitBuildInfo.h"

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
        QString copyrightInfo = tr("<p>Based on Qt version %1.<br>"
                                   "Copyright 2019-%2 OpenVPI. All rights reserved.</p>")
                                    .arg(QLatin1String(QT_VERSION_STR), QLatin1String(CHORUSKIT_BUILD_YEAR));

        // const char *compiler =
        //     qstrcmp("MSVC", CHORUSKIT_BUILD_COMPILER_ID) ? CHORUSKIT_BUILD_COMPILER_ID : "Microsoft Visual C++";

        QString buildInfo =
            tr("<h3>Build Information</h3>"
               "<p>"
               "Version: %1<br>"
               "Branch: %2<br>"
               "Commit: %3<br>"
               "Build date: %4<br>"
               "Toolchain: %5 %6 %7"
               "</p>")
                .arg(QApplication::applicationVersion(), QLatin1String(CHORUSKIT_GIT_BRANCH),
                     QLatin1String(CHORUSKIT_GIT_COMMIT_HASH), QLatin1String(CHORUSKIT_BUILD_DATE_TIME),
                     QLatin1String(CHORUSKIT_BUILD_COMPILER_ARCH), QLatin1String(CHORUSKIT_BUILD_COMPILER_ID),
                     QLatin1String(CHORUSKIT_BUILD_COMPILER_VERSION));

        QString aboutInfo = tr("<h3>About</h3>"
                               "<p>DiffScope is a kind of implementation of DiffSinger graphical editing tool, "
                               "included in ChorusKit toolset.</p>");

        QString licenseInfo = tr(
            "<h3>License</h3>"
            "<p>Licensed under the Apache License, Version 2.0.<br>"
            R"(You may obtain a copy of the License at <a href="http://www.apache.org/licenses/LICENSE-2.0">apache.org/licenses</a>.</p>)"
            "<p>This application is distributed "
            "<b>AS IS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND</b>, either express or implied.</p>");

        QString translatedTextAboutQtCaption =
            tr("<h2>ChorusKit DiffScope</h2>%1%2%3%4").arg(copyrightInfo, buildInfo, aboutInfo, licenseInfo);

        QMessageBox msgBox(parent);
        msgBox.setWindowTitle(tr("About %1").arg(qAppName()));
        msgBox.setText(translatedTextAboutQtCaption);

        double ratio = (msgBox.screen()->logicalDotsPerInch() / QMOs::unitDpi());

        QIcon icon(":/svg/app/diffsinger.svg");
        if (!icon.isNull())
            msgBox.setIconPixmap(icon.pixmap(QSize(40, 40) * ratio));

        auto layout = qobject_cast<QGridLayout *>(msgBox.layout());
        auto horizontalSpacer = new QSpacerItem(ratio * 500, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
        layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
        msgBox.exec();
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

    void ICore::showHome() {
        auto inst = IHomeWindow::instance();
        if (inst) {
            QMView::bringWindowToForeground(inst->window());
            return;
        }
        instance()->windowSystem()->createWindow("home");
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