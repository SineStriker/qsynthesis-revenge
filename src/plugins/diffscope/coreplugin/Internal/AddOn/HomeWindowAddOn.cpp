#include "HomeWindowAddOn.h"

#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "Window/IHomeWindow.h"

#include "Internal/Widgets/HomeRecentWidget.h"

#include "ICore.h"

#include <QMDecorator.h>

#include <Internal/plugindialog.h>

namespace Core {

    namespace Internal {

        bool HomeWindowAddOnFactory::predicate(IWindow *handle) const {
            return handle->id() == "home";
        }

        IWindowAddOn *HomeWindowAddOnFactory::create(QObject *parent) {
            return new HomeWindowAddOn(parent);
        }

        HomeWindowAddOn::HomeWindowAddOn(QObject *parent) : IWindowAddOn(parent) {
        }

        HomeWindowAddOn::~HomeWindowAddOn() {
        }

        void HomeWindowAddOn::initialize() {
            initActions();

            auto iWin = qobject_cast<IHomeWindow *>(this->windowHandle());

            // Add recent widget
            auto recentWidget = new HomeRecentWidget();
            recentWidgetButton = iWin->addNavWidget(recentWidget);

            auto recentTopWidget = recentWidget->topWidget;
            connect(recentTopWidget->newButton, &QPushButton::clicked, this, &HomeWindowAddOn::_q_newButtonClicked);
            connect(recentTopWidget->openButton, &QPushButton::clicked, this, &HomeWindowAddOn::_q_openButtonClicked);

            // Add to object pool to accept buttons
            iWin->addWidget("core.recentWidget.buttonsLayout", recentTopWidget);

            qIDec->installLocale(this, {{}}, _LOC(HomeWindowAddOn, this));
        }

        void HomeWindowAddOn::extensionsInitialized() {
        }

        void HomeWindowAddOn::reloadStrings() {
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

            recentWidgetButton->setText(tr("Recent"));
        }

        void HomeWindowAddOn::initActions() {
            auto iWin = windowHandle();

            fileItem = new ActionItem("core.File", new QMenu(), this);
            helpItem = new ActionItem("core.Help", new QMenu(), this);

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
                docMgr->openFileBrowse(docMgr->docType("core.Dspx"));
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

        void HomeWindowAddOn::_q_newButtonClicked() {
            newFileItem->action()->trigger();
        }

        void HomeWindowAddOn::_q_openButtonClicked() {
            openFileItem->action()->trigger();
        }

    }

}