#include "HomeWindowAddOn.h"

#include <QApplication>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>

#include "ICore.h"
#include "Internal/Widgets/HomeRecentWidget.h"
#include "Internal/plugindialog.h"
#include "Window/IHomeWindow.h"

#include <QMDecoratorV2.h>


namespace Core {

    namespace Internal {

        bool HomeWindowAddOnFactory::predicate(IWindow *handle) const {
            return handle->id() == "home";
        }

        IWindowAddOn *HomeWindowAddOnFactory::create(QObject *parent) {
            return new HomeWindowAddOn(parent);
        }

        HomeWindowAddOn::HomeWindowAddOn(QObject *parent) : CoreWindowAddOn(parent) {
        }

        HomeWindowAddOn::~HomeWindowAddOn() {
        }

        void HomeWindowAddOn::initialize() {
            CoreWindowAddOn::initialize();

            initActions();

            auto iWin = windowHandle()->cast<IHomeWindow>();

            // Add recent widget
            auto recentWidget = new HomeRecentWidget();
            recentWidgetButton = iWin->addNavWidget(recentWidget);

            auto recentTopWidget = recentWidget->topWidget;
            connect(recentTopWidget, &HomeRecentTopFrame::newRequested, this, &HomeWindowAddOn::_q_newButtonClicked);
            connect(recentTopWidget, &HomeRecentTopFrame::openRequested, this, &HomeWindowAddOn::_q_openButtonClicked);

            auto recentBottomWidget = recentWidget->bottomWidget;
            connect(recentBottomWidget, &HomeRecentBottomFrame::openFileRequested, this, &HomeWindowAddOn::_q_openFileRequested);

            qIDec->installLocale(this, _LOC(HomeWindowAddOn, this));

            // Extension Point: add buttons to recent widget
            iWin->addWidget("core.recentWidget.buttonBar", recentTopWidget);                       // Reflection
            iWin->addObject("core.recentWidget.buttonBarInterface", recentTopWidget->buttonBar()); // Interface
        }

        void HomeWindowAddOn::extensionsInitialized() {
            auto iWin = windowHandle()->cast<IHomeWindow>();

            iWin->removeWidget("core.recentWidget.buttonBar");
            iWin->removeObjects("core.recentWidget.buttonBarInterface");

            CoreWindowAddOn::extensionsInitialized();
        }

        void HomeWindowAddOn::reloadStrings() {
            recentWidgetButton->setText(tr("Recent"));
        }

        void HomeWindowAddOn::initActions() {
        }

        void HomeWindowAddOn::_q_newButtonClicked() {
            newFileItem->action()->trigger();
        }

        void HomeWindowAddOn::_q_openButtonClicked() {
            openFileItem->action()->trigger();
        }

        void HomeWindowAddOn::_q_openFileRequested(const QString &fileName) {
            openFile(fileName);
        }

    }

}