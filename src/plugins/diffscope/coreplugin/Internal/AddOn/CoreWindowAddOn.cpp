#include "CoreWindowAddOn.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QMessageBox>

#include <QMDecoratorV2.h>

#include "ICore.h"
#include "Internal/plugindialog.h"
#include "Window/ICoreWindow.h"

#include "QsFrameworkNamespace.h"

namespace Core::Internal {

    CoreWindowAddOn::CoreWindowAddOn(QObject *parent) : IWindowAddOn(parent) {
    }

    CoreWindowAddOn::~CoreWindowAddOn() {
    }

    void CoreWindowAddOn::initialize() {
        auto iWin = windowHandle();

        initActions();
        reloadRecentMenu();

        auto docMgr = ICore::instance()->documentSystem();

        // Add drag handler
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

        preferenceMenuItem->setText(tr("Preferences"));
        settingsItem->setText(tr("Se&ttings..."));
        colorThemesItem->setText(tr("&Color Themes..."));
        languagesItem->setText(tr("&Locales And Languages..."));

        welcomeGroupItem->setText(tr("Welcome Actions"));
        showHomeItem->setText(tr("Show Home"));
        findActionItem->setText(tr("&Find Action..."));

        aboutGroupItem->setText(tr("About Actions"));
        aboutPluginsItem->setText(tr("About Plugins"));
        aboutAppItem->setText(tr("&About %1").arg(qApp->applicationName()));
        aboutQtItem->setText(tr("About Qt"));
    }

    void CoreWindowAddOn::initActions() {
        auto iWin = windowHandle()->cast<ICoreWindow>();
        auto win = iWin->window();

        fileItem = new ActionItem("core.File", ICore::createCoreMenu(win), this);
        helpItem = new ActionItem("core.Help", ICore::createCoreMenu(win), this);

        openGroupItem = new ActionItem("core.OpenGroup", new QActionGroup(this), this);
        newFileItem = new ActionItem("core.NewFile", new QAction(this), this);
        openFileItem = new ActionItem("core.OpenFile", new QAction(this), this);
        openRecentItem = new ActionItem("core.OpenRecent", ICore::createCoreMenu(win), this);

        preferenceMenuItem = new ActionItem("core.PreferenceMenu", ICore::createCoreMenu(win), this);
        settingsItem = new ActionItem("core.Settings", new QAction(this), this);
        colorThemesItem = new ActionItem("core.ColorThemes", new QAction(this), this);
        languagesItem = new ActionItem("core.Languages", new QAction(this), this);

        aboutGroupItem = new ActionItem("core.AboutGroup", new QActionGroup(this), this);
        aboutPluginsItem = new ActionItem("core.AboutPlugins", new QAction(this), this);
        aboutAppItem = new ActionItem("core.AboutApp", new QAction(this), this);
        aboutQtItem = new ActionItem("core.AboutQt", new QAction(this), this);

        welcomeGroupItem = new ActionItem("core.WelcomeGroup", new QActionGroup(this), this);
        showHomeItem = new ActionItem("core.ShowHome", new QAction(this), this);
        findActionItem = new ActionItem("core.FindAction", new QAction(this), this);

        connect(newFileItem->action(), &QAction::triggered, this, [this, iWin]() {
            //
            qDebug() << "New";

            iWin->showMenuInPalette(fileItem->menu(), false);

            //            auto action = new QAction("Test");
            //            action->setShortcut(QKeySequence("Ctrl+N"));
            //            helpItem->menu()->addAction(action);
        });

        connect(openFileItem->action(), &QAction::triggered, this, [iWin]() {
            auto docMgr = ICore::instance()->documentSystem();
            auto spec = docMgr->docType(qApp->property("projectDocTypeId").toString());
            if (!spec) {
                QMessageBox::critical(
                    iWin->window(), ICore::mainTitle(),
                    tr("Can't find the default editor of %1 project file!").arg(qApp->applicationName()));
                return;
            }

            if (docMgr->openFileBrowse(iWin->window(), spec)) {
                if (qApp->property("closeHomeOnOpen").toBool() && iWin->id() == "home") {
                    QTimer::singleShot(0, iWin->window(), &QWidget::close);
                }
            }
        });

        connect(settingsItem->action(), &QAction::triggered, this, [win]() {
            ICore::instance()->showSettingsDialog("core.Settings", win); //
        });

        connect(colorThemesItem->action(), &QAction::triggered, this, [iWin]() {
            iWin->selectColorThemes(); //
        });

        connect(languagesItem->action(), &QAction::triggered, this, [iWin]() {
            iWin->selectTranslations(); //
        });

        connect(aboutPluginsItem->action(), &QAction::triggered, this, [win]() {
            Internal::PluginDialog dlg(win);
            dlg.exec();
        });

        connect(aboutAppItem->action(), &QAction::triggered, this, [win]() {
            ICore::aboutApp(win); //
        });

        connect(aboutQtItem->action(), &QAction::triggered, this, [win]() {
            QMessageBox::aboutQt(win); //
        });

        connect(showHomeItem->action(), &QAction::triggered, this, [this]() {
            ICore::showHome(); //
        });

        connect(findActionItem->action(), &QAction::triggered, this, [iWin]() { iWin->showAllActions(); });

        iWin->addActionItems({
            fileItem,
            helpItem,
            openGroupItem,
            newFileItem,
            openFileItem,
            openRecentItem,
            preferenceMenuItem,
            settingsItem,
            colorThemesItem,
            languagesItem,
            aboutGroupItem,
            aboutPluginsItem,
            aboutAppItem,
            aboutQtItem,
            welcomeGroupItem,
            showHomeItem,
            findActionItem,
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
