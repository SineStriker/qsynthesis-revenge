#include "HomeWindowAddOn.h"

#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QPushButton>
#include <QVBoxLayout>

#include "Window/IHomeWindow.h"

#include "ICore.h"
#include "Internal/TestDialogPage.h"

namespace Core {

    namespace Internal {

        bool HomeWindowAddOnFactory::predicate(IWindow *iWindow) const {
            return iWindow->id() == "home";
        }

        IWindowAddOn *HomeWindowAddOnFactory::create(QObject *parent) {
            return new HomeWindowAddOn(parent);
        }

        HomeWindowAddOn::HomeWindowAddOn(QObject *parent) : IWindowAddOn(parent) {
        }

        HomeWindowAddOn::~HomeWindowAddOn() {
        }

        void HomeWindowAddOn::initialize() {
            auto iWin = qobject_cast<IHomeWindow *>(this->handle());
            ICore::instance()->dialogHelper()->addDialogPage(new TestDialogPage);
            auto testLayout = new QVBoxLayout;
            auto testTextInput = new QLineEdit;
            auto testDialogPage =
                qobject_cast<TestDialogPage *>(ICore::instance()->dialogHelper()->getDialogPage("test"));
            connect(testTextInput, &QLineEdit::textChanged, testDialogPage, &TestDialogPage::setLabelText);
            connect(testDialogPage, &TestDialogPage::accepted, testTextInput,
                    [=]() { testTextInput->setText(testDialogPage->getEditText()); });
            testTextInput->setText("114514");
            auto testModalDialog = new QPushButton("Test modal dialog");
            auto testModelessDialog = new QPushButton("Test modeless dialog");
            auto testAcceptCloseDialog = new QPushButton("Test accept and close dialog");
            auto testRejectCloseDialog = new QPushButton("Test reject and close dialog");
            connect(testModalDialog, &QPushButton::clicked,
                    [=]() { ICore::instance()->dialogHelper()->showDialog("test", testModalDialog); });
            connect(testModelessDialog, &QPushButton::clicked, [=]() {
                ICore::instance()->dialogHelper()->showModelessDialog(
                    "test", testModelessDialog,
                    DialogHelper::OkButton | DialogHelper::CancelButton | DialogHelper::ApplyButton);
            });
            connect(testAcceptCloseDialog, &QPushButton::clicked,
                    [=]() { ICore::instance()->dialogHelper()->closeDialog("test", QDialog::Accepted); });
            connect(testRejectCloseDialog, &QPushButton::clicked,
                    [=]() { ICore::instance()->dialogHelper()->closeDialog("test", QDialog::Rejected); });
            testLayout->addWidget(testTextInput);
            testLayout->addWidget(testModalDialog);
            testLayout->addWidget(testModelessDialog);
            testLayout->addWidget(testAcceptCloseDialog);
            testLayout->addWidget(testRejectCloseDialog);
            testLayout->addWidget(new QLabel("22222222222222222"));
            auto testWidget = new QWidget;
            testWidget->setLayout(testLayout);
            auto btn = iWin->addNavWidget(testWidget);
            btn->setText("123456");

            reloadMenuBar();
        }

        void HomeWindowAddOn::extensionsInitialized() {
        }

        void HomeWindowAddOn::reloadMenuBar() {
            auto fileItem = new ActionItem("home_File", new QMenu("File(&F)"));
            auto editItem = new ActionItem("home_Edit", new QMenu("Edit(&E)"));
            auto helpItem = new ActionItem("home_Help", new QMenu("Help(&H)"));

            auto openGroupItem = new ActionItem("home_OpenGroup", new QActionGroup(this));
            auto newFileItem = new ActionItem("home_NewFile", new QAction("New File"));
            auto openFileItem = new ActionItem("home_OpenFile", new QAction("Open File"));

            auto preferenceGroupItem = new ActionItem("home_PreferenceGroup", new QActionGroup(this));
            auto settingsItem = new ActionItem("home_Settings", new QAction("Settings"));

            connect(settingsItem->action(), &QAction::triggered, this, [this]() {
                Q_UNUSED(this);
                ICore::instance()->settingCatalog()->showDialog("core_Settings"); //
            });

            ICore::instance()
                ->actionSystem()
                ->context("home_MainMenu")
                ->buildMenuBarWithState(
                    {
                        fileItem,
                        editItem,
                        helpItem,
                        openGroupItem,
                        newFileItem,
                        openFileItem,
                        preferenceGroupItem,
                        settingsItem,
                    },
                    handle()->menuBar());
        }
    }

}