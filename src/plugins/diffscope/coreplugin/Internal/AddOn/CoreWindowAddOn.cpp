#include "CoreWindowAddOn.h"

#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include <QMDecoratorV2.h>

#include "ICore.h"
#include "Internal/plugindialog.h"
#include "Window/ICoreWindow.h"

namespace Core::Internal {

    CoreWindowAddOn::CoreWindowAddOn(QObject *parent) : IWindowAddOn(parent) {
    }

    CoreWindowAddOn::~CoreWindowAddOn() {
    }

    void CoreWindowAddOn::initialize() {
        initActions();
        reloadRecentMenu();

        auto docMgr = ICore::instance()->documentSystem();

        // Add drag handler
        auto iWin = windowHandle();
        for (const auto &suffix : docMgr->supportedExtensions())
            iWin->setDragFileHandler(suffix, iWin, "openFile");

        connect(docMgr, &DocumentSystem::recentFilesChanged, this, &CoreWindowAddOn::_q_recentFilesChanged);

        qIDec->installLocale(this, _LOC(CoreWindowAddOn, this));
    }

    void CoreWindowAddOn::extensionsInitialized() {
    }

    void CoreWindowAddOn::reloadStrings() {
        fileItem->setText(tr("&File"));
        helpItem->setText(tr("&Help"));

        openGroupItem->setText(tr("Open Actions"));
        newFileItem->setText(tr("&New"));
        openFileItem->setText(tr("&Open..."));
        openRecentItem->setText(tr("Open Recent"));

        preferenceGroupItem->setText(tr("Preference Actions"));
        settingsItem->setText(tr("Se&ttings..."));

        aboutGroupItem->setText(tr("About Actions"));
        aboutPluginsItem->setText(tr("About Plugins"));
        aboutAppItem->setText(tr("&About %1").arg(qAppName()));
        aboutQtItem->setText(tr("About Qt"));
    }

    void CoreWindowAddOn::initActions() {
        auto iWin = windowHandle();
        auto win = iWin->window();

        fileItem = new ActionItem("core.File", ICore::createCoreMenu(win), this);
        helpItem = new ActionItem("core.Help", ICore::createCoreMenu(win), this);

        openGroupItem = new ActionItem("core.OpenGroup", new QActionGroup(this), this);
        newFileItem = new ActionItem("core.NewFile", new QAction(this), this);
        openFileItem = new ActionItem("core.OpenFile", new QAction(this), this);
        openRecentItem = new ActionItem("core.OpenRecent", ICore::createCoreMenu(win), this);

        preferenceGroupItem = new ActionItem("core.PreferenceGroup", new QActionGroup(this), this);
        settingsItem = new ActionItem("core.Settings", new QAction(this), this);

        aboutGroupItem = new ActionItem("core.AboutGroup", new QActionGroup(this), this);
        aboutPluginsItem = new ActionItem("core.AboutPlugins", new QAction(this), this);
        aboutAppItem = new ActionItem("core.AboutApp", new QAction(this), this);
        aboutQtItem = new ActionItem("core.AboutQt", new QAction(this), this);

        connect(newFileItem->action(), &QAction::triggered, this, [this]() {
            //
            qDebug() << "New";

            auto action = new QAction("Test");
            action->setShortcut(QKeySequence("Ctrl+N"));
            helpItem->menu()->addAction(action);
        });

        connect(openFileItem->action(), &QAction::triggered, this, [this]() {
            auto docMgr = ICore::instance()->documentSystem();
            auto spec = docMgr->docType(qApp->property("projectDocTypeId").toString());
            if (!spec) {
                QMessageBox::critical(windowHandle()->window(), ICore::mainTitle(),
                                      tr("Can't find the default editor of %1 project file!").arg(qAppName()));
                return;
            }

            auto iWin = windowHandle();
            if (docMgr->openFileBrowse(spec)) {
                if (qApp->property("closeHomeOnOpen").toBool() && iWin->id() == "home") {
                    QTimer::singleShot(0, iWin->window(), &QWidget::close);
                }
            }
        });

        connect(settingsItem->action(), &QAction::triggered, this, [this]() {
            ICore::instance()->showSettingsDialog("core.Settings", windowHandle()->window()); //
        });

        connect(aboutPluginsItem->action(), &QAction::triggered, this, [this]() {
            Internal::PluginDialog dlg(windowHandle()->window());
            dlg.exec();
        });

        connect(aboutAppItem->action(), &QAction::triggered, this, [this]() {
            ICore::aboutApp(windowHandle()->window()); //
        });

        connect(aboutQtItem->action(), &QAction::triggered, this, [this]() {
            QMessageBox::aboutQt(windowHandle()->window()); //
        });

        iWin->addActionItems({
            fileItem,
            helpItem,
            openGroupItem,
            newFileItem,
            openFileItem,
            openRecentItem,
            preferenceGroupItem,
            settingsItem,
            aboutGroupItem,
            aboutPluginsItem,
            aboutAppItem,
            aboutQtItem,
        });
    }

    void CoreWindowAddOn::reloadRecentMenu() {
        auto docMgr = ICore::instance()->documentSystem();
        auto menu = openRecentItem->menu();

        menu->clear();

        int cnt = 0;
        for (const auto &file : docMgr->recentFiles()) {
            auto action = menu->addAction(QDir::toNativeSeparators(file));
            action->setData(file);
            connect(action, &QAction::triggered, this, [this]() {
                windowHandle()->cast<ICoreWindow>()->openFile(qobject_cast<QAction *>(sender())->data().toString()); //
            });
            cnt++;
            if (cnt >= 10)
                break;
        }

        if (cnt > 0) {
            menu->addSeparator();

            auto action = new QAction(menu);
            menu->addAction(action);

            auto slot = [action](const QString &locale) {
                action->setText(tr("Clear recent list")); //
            };

            connect(qIDec, &QMCoreDecoratorV2::localeChanged, action, slot);
            connect(action, &QAction::triggered, docMgr, &DocumentSystem::clearRecentFiles);

            slot(qIDec->locale());
        } else {
            auto action = new QAction(menu);
            action->setDisabled(true);
            menu->addAction(action);

            auto slot = [action](const QString &locale) {
                action->setText(tr("Null")); //
            };

            connect(qIDec, &QMCoreDecoratorV2::localeChanged, action, slot);
            slot(qIDec->locale());
        }
    }

    void CoreWindowAddOn::_q_recentFilesChanged() {
        reloadRecentMenu();
    }

}
