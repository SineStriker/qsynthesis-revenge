#include "ParamsAddOn.h"

#include <QMDecoratorV2.h>

#include <coreplugin/Window/IProjectWindow.h>

namespace Params::Internal {

    using namespace Core;

    bool ParamsAddOnFactory::predicate(Core::IWindow *handle) const {
        return handle->id() == "project";
    }
    Core::IWindowAddOn *ParamsAddOnFactory::create(QObject *parent) {
        return new ParamsAddOn(parent);
    }

    ParamsAddOn::ParamsAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }

    ParamsAddOn::~ParamsAddOn() {
    }

    void ParamsAddOn::initialize() {
        auto iWin = windowHandle()->cast<Core::IProjectWindow>();

        initActions();

        // Add phoneme panel
        paramsPanel = new Internal::ParamsPanel();
        paramsButton = iWin->addFloatingPanel("edit.paramsPanel", paramsPanel, nullptr);

        qIDec->installLocale(this, _LOC(ParamsAddOn, this));
    }

    void ParamsAddOn::extensionsInitialized() {
    }

    void ParamsAddOn::reloadStrings() {
        paramsPanelVisibleItem->setText(tr("Parameters"));
        paramsButton->setText(tr("Parameters"));
    }

    void ParamsAddOn::initActions() {
        auto iWin = windowHandle()->cast<IProjectWindow>();
        auto win = iWin->window();

        // View
        paramsPanelVisibleItem = new ActionItem("params.ParamsPanelVisible", new QAction(this), this);

        paramsPanelVisibleItem->action()->setCheckable(true);
        iWin->addCheckable("edit.paramsPanel", paramsPanelVisibleItem->action());

        iWin->addActionItems({
            paramsPanelVisibleItem,
        });
    }

}