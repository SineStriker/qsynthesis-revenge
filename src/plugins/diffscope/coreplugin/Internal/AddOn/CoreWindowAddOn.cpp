#include "CoreWindowAddOn.h"

#include <QCoreApplication>
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

            auto spec = docMgr->supportedDocTypes("dspx");
            if (spec.isEmpty()) {
                QMessageBox::critical(windowHandle()->window(), ICore::mainTitle(), tr("!"));
                return;
            }

            if (docMgr->openFileBrowse(spec.front())) {
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
            preferenceGroupItem,
            settingsItem,
            aboutGroupItem,
            aboutPluginsItem,
            aboutAppItem,
            aboutQtItem,
        });
    }

}
