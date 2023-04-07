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
#include "Internal/GeneralOptionsPage.h"
#include "Internal/TestDialogPage.h"
#include "Internal/TestOptionsPage.h"

#include <QMDecorator.h>

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

            ICore::instance()->preferenceRegistry()->addItem(new GeneralOptionsPage);
            ICore::instance()->preferenceRegistry()->addItem(new TestOptionsPage, "ChorusKit:General");
            auto testPreferenceDialog = new QPushButton("Test preference dialog");
            connect(testPreferenceDialog, &QPushButton::clicked, [=]() {
                ICore::instance()->preferenceRegistry()->showPreferenceDialog(testPreferenceDialog,
                                                                              "ChorusKit:General");
            });

            testLayout->addWidget(testTextInput);
            testLayout->addWidget(testModalDialog);
            testLayout->addWidget(testModelessDialog);
            testLayout->addWidget(testAcceptCloseDialog);
            testLayout->addWidget(testRejectCloseDialog);
            testLayout->addWidget(testPreferenceDialog);
            testLayout->addWidget(new QLabel("22222222222222222"));
            auto testWidget = new QWidget;
            testWidget->setLayout(testLayout);
            auto btn = iWin->addNavWidget(testWidget);
            btn->setText("123456");

            reloadMenuBar();

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

        void HomeWindowAddOn::reloadMenuBar() {
            fileItem = new ActionItem("home_File", new QMenu());
            editItem = new ActionItem("home_Edit", new QMenu());
            helpItem = new ActionItem("home_Help", new QMenu());

            openGroupItem = new ActionItem("home_OpenGroup", new QActionGroup(this));
            newFileItem = new ActionItem("home_NewFile", new QAction());
            openFileItem = new ActionItem("home_OpenFile", new QAction());

            preferenceGroupItem = new ActionItem("home_PreferenceGroup", new QActionGroup(this));
            settingsItem = new ActionItem("home_Settings", new QAction());

            aboutGroupItem = new ActionItem("home_AboutGroup", new QActionGroup(this));
            aboutAppItem = new ActionItem("home_AboutApp", new QAction(this));
            aboutQtItem = new ActionItem("home_AboutQt", new QAction(this));

            connect(newFileItem->action(), &QAction::triggered, this, [this]() {
                ICore::instance()->showWizardDialog("", handle()->window()); //
            });

            connect(settingsItem->action(), &QAction::triggered, this, [this]() {
                ICore::instance()->showSettingsDialog("core_Settings", handle()->window()); //
            });

            connect(aboutAppItem->action(), &QAction::triggered, this, [this]() {
                ICore::instance()->aboutApp(handle()->window()); //
            });

            connect(aboutQtItem->action(), &QAction::triggered, this, [this]() {
                QMessageBox::aboutQt(handle()->window()); //
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
                        aboutGroupItem,
                        aboutAppItem,
                        aboutQtItem,
                    },
                    handle()->menuBar());
        }
    }

}