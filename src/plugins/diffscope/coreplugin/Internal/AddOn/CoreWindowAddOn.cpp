#include "CoreWindowAddOn.h"

#include <QCoreApplication>
#include <QDir>
#include <QMessageBox>

#include <QMDecoratorV2.h>

#include "ICore.h"
#include "Internal/plugindialog.h"

namespace Core::Internal {

    CoreWindowAddOn::CoreWindowAddOn(QObject *parent) : IWindowAddOn(parent) {
    }

    CoreWindowAddOn::~CoreWindowAddOn() {
    }

    void CoreWindowAddOn::initialize() {
        initActions();
        reloadRecentMenu();

        auto docMgr = ICore::instance()->documentSystem();
        connect(docMgr, &DocumentSystem::recentFilesChanged, this, &CoreWindowAddOn::_q_recentFilesChanged);

        qIDec->installLocale(this, _LOC(CoreWindowAddOn, this));
    }

    void CoreWindowAddOn::extensionsInitialized() {
    }

    void CoreWindowAddOn::reloadStrings() {
        fileItem->setText(tr("File(&F)"));
        helpItem->setText(tr("Help(&H)"));

        openGroupItem->setText(tr("Open Actions"));
        newFileItem->setText(tr("New"));
        openFileItem->setText(tr("Open"));
        openRecentItem->setText(tr("Open Recent"));

        preferenceGroupItem->setText(tr("Preference Actions"));
        settingsItem->setText(tr("Settings"));

        aboutGroupItem->setText(tr("About Actions"));
        aboutPluginsItem->setText(tr("About Plugins"));
        aboutAppItem->setText(tr("About %1").arg(qAppName()));
        aboutQtItem->setText(tr("About Qt"));
    }

    void CoreWindowAddOn::initActions() {
        auto iWin = windowHandle();
        auto win = iWin->window();

        fileItem = new ActionItem("core.File", ICore::createCoreMenu(win), this);
        helpItem = new ActionItem("core.Help", ICore::createCoreMenu(win), this);

        openGroupItem = new ActionItem("core.OpenGroup", new QActionGroup(this), this);
        newFileItem = new ActionItem("core.NewFile", new QAction(), this);
        openFileItem = new ActionItem("core.OpenFile", new QAction(), this);
        openRecentItem = new ActionItem("core.OpenRecent", ICore::createCoreMenu(win), this);

        preferenceGroupItem = new ActionItem("core.PreferenceGroup", new QActionGroup(this), this);
        settingsItem = new ActionItem("core.Settings", new QAction(), this);

        aboutGroupItem = new ActionItem("core.AboutGroup", new QActionGroup(this), this);
        aboutPluginsItem = new ActionItem("core.AboutPlugins", new QAction(this), this);
        aboutAppItem = new ActionItem("core.AboutApp", new QAction(this), this);
        aboutQtItem = new ActionItem("core.AboutQt", new QAction(this), this);

        connect(newFileItem->action(), &QAction::triggered, this, [this]() {
            //
        });

        connect(openFileItem->action(), &QAction::triggered, this, [this]() {
            auto docMgr = ICore::instance()->documentSystem();
            auto spec = getSpec();
            if (!spec) {
                return;
            }
            if (docMgr->openFileBrowse(spec)) {
                windowHandle()->window()->close();
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

        iWin->setDragFileHandler("dspx", this, "openFile");
    }

    DocumentSpec *CoreWindowAddOn::getSpec() const {
        auto docMgr = ICore::instance()->documentSystem();
        auto specs = docMgr->supportedDocTypes("dspx");
        if (specs.isEmpty()) {
            QMessageBox::critical(windowHandle()->window(), ICore::mainTitle(), tr("!"));
            return nullptr;
        }
        return specs.front();
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
                openFile(qobject_cast<QAction *>(sender())->data().toString()); //
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

    void CoreWindowAddOn::openFile(const QString &path) {
        auto spec = getSpec();
        if (!spec) {
            return;
        }
        if (spec->open(path)) {
            windowHandle()->window()->close();
        }
    }

    void CoreWindowAddOn::_q_recentFilesChanged() {
        reloadRecentMenu();
    }

}
