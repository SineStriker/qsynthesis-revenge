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

#include "ICore.h"

#include <QMDecorator.h>

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
            auto iWin = qobject_cast<IHomeWindow *>(this->windowHandle());

            auto btn = iWin->addNavWidget(new QLabel("First Page"));
            btn->setText("123456");

            initActions();

            qIDec->installLocale(this, {{}}, _LOC(HomeWindowAddOn, this));
        }

        void HomeWindowAddOn::extensionsInitialized() {
        }

        void HomeWindowAddOn::reloadStrings() {
            fileItem->setText(tr("File(&F)"));
            editItem->setText(tr("Edit(&E)"));
            helpItem->setText(tr("Help(&H)"));

            openGroupItem->setText(tr("Open Actions"));
            newFileItem->setText(tr("New"));
            openFileItem->setText(tr("Open"));

            preferenceGroupItem->setText(tr("Preference Actions"));
            settingsItem->setText(tr("Settings"));

            aboutGroupItem->setText(tr("About Actions"));
            aboutAppItem->setText(tr("About %1").arg(qAppName()));
            aboutQtItem->setText(tr("About Qt"));
        }

        void HomeWindowAddOn::initActions() {
            auto iWin = windowHandle();

            fileItem = new ActionItem("core_File", new QMenu());
            editItem = new ActionItem("core_Edit", new QMenu());
            helpItem = new ActionItem("core_Help", new QMenu());

            openGroupItem = new ActionItem("core_OpenGroup", new QActionGroup(this));
            newFileItem = new ActionItem("core_NewFile", new QAction());
            openFileItem = new ActionItem("core_OpenFile", new QAction());

            preferenceGroupItem = new ActionItem("core_PreferenceGroup", new QActionGroup(this));
            settingsItem = new ActionItem("core_Settings", new QAction());

            aboutGroupItem = new ActionItem("core_AboutGroup", new QActionGroup(this));
            aboutAppItem = new ActionItem("core_AboutApp", new QAction(this));
            aboutQtItem = new ActionItem("core_AboutQt", new QAction(this));

            connect(newFileItem->action(), &QAction::triggered, this, [this]() {
                ICore::instance()->showWizardDialog("", windowHandle()->window()); //
            });

            connect(settingsItem->action(), &QAction::triggered, this, [this]() {
                ICore::instance()->showSettingsDialog("core_Settings", windowHandle()->window()); //
            });

            connect(aboutAppItem->action(), &QAction::triggered, this, [this]() {
                ICore::aboutApp(windowHandle()->window()); //
            });

            connect(aboutQtItem->action(), &QAction::triggered, this, [this]() {
                QMessageBox::aboutQt(windowHandle()->window()); //
            });

            iWin->addActionItems({
                fileItem,
                editItem,
                helpItem,
                openGroupItem,
                newFileItem,
                openFileItem,
                preferenceGroupItem,
                settingsItem,
                aboutGroupItem,
                aboutAppItem,
                aboutQtItem,
            });
        }
    }

}