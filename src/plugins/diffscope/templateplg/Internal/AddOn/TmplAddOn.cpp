#include "TmplAddOn.h"
#include "Dialogs/TmplDialog.h"
#include <QMDecoratorV2.h>

#include <QDebug>
#include <QMetaObject>

#include <coreplugin/ICore.h>

namespace TemplatePlg {

    using namespace Core;

    namespace Internal {

        bool TmplAddOnFactory::predicate(Core::IWindow *handle) const {
            return handle->id() == "home" || handle->id() == "project";
        }

        Core::IWindowAddOn *TmplAddOnFactory::create(QObject *parent) {
            return new TmplAddOn(parent);
        }

        TmplAddOn::TmplAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
        }

        TmplAddOn::~TmplAddOn() {
        }

        void TmplAddOn::initialize() {
            auto iWin = windowHandle();

            initActions();

            qIDec->installLocale(this, _LOC(TmplAddOn, this));
        }

        void TmplAddOn::extensionsInitialized() {
        }

        void TmplAddOn::reloadStrings() {
            templateGroupItem->setText(tr("Template Group"));
            tmplMenu->setText(tr("Tmpl Menu"));
            tmplImpItem->setText(tr("Tmpl Import"));
        }

        void TmplAddOn::initActions() {
            auto iWin = windowHandle();
            auto win = iWin->window();

            //            // Add dialog
            //            TmplDialog dialog(win, tr("Info"), tr("Are you sure you want to delete it?"), tr("ok"),
            //            tr("cancel")); if (dialog.exec() == QDialog::Accepted) {
            //                qDebug() << "TmplAddOn: TmplDialog select ok";
            //            } else {
            //                qDebug() << "TmplAddOn: TmplDialog select cancel";
            //            }

            templateGroupItem = new ActionItem("template.TmplGroup", new QActionGroup(this), this);

            tmplMenu = new ActionItem("template.TmplMenu", ICore::createCoreMenu(win), this);
            tmplImpItem = new ActionItem("template.TmplImport", new QAction(this), this);

            connect(tmplImpItem->action(), &QAction::triggered, this, &TmplAddOn::_q_tmplButtonClicked);

            iWin->addActionItems({
                templateGroupItem,
                tmplMenu,
                tmplImpItem,
            });
        }

        void TmplAddOn::_q_tmplButtonClicked() {
            qDebug() << "TmplAddOn: The template import button has been clicked";
        }
    }
}