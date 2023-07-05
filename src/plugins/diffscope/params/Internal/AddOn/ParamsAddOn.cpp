#include "ParamsAddOn.h"

#include <QMDecoratorV2.h>

#include <coreplugin/Window/IProjectWindow.h>

namespace Params::Internal {

    using namespace Core;

    static const char *paramsKey = "edit.paramsPanel";

    ParamsAddOn::ParamsAddOn(QObject *parent) : Core::IWindowAddOn(parent) {
    }

    ParamsAddOn::~ParamsAddOn() {
    }

    void ParamsAddOn::initialize() {
        auto iWin = windowHandle()->cast<Core::IProjectWindow>();

        initActions();

        auto pianoRoll = iWin->pianoRoll();

        // Add phoneme panel
        paramsPanel = new Internal::ParamsPanel();
        paramsButton = pianoRoll->addFloatingPanel(paramsKey, paramsPanel, nullptr);
        paramsButton->setObjectName("params-button");

        connect(pianoRoll, &PianoRoll::floatingPanelStateChanged, this,
                [this](const QString &id, PianoRoll::FloatingPanelState state) {
                    if (id == paramsKey) {
                        paramsPanelVisibleItem->action()->setChecked(state != PianoRoll::Hidden);
                    }
                });

        connect(paramsPanelVisibleItem->action(), &QAction::toggled, this, [pianoRoll](bool checked) {
            if (checked == (pianoRoll->floatingPanelState(paramsKey) == PianoRoll::Hidden)) {
                pianoRoll->setFloatingPanelState(paramsKey, checked ? PianoRoll::Normal : PianoRoll::Hidden);
            }
        });

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
        iWin->addCheckable(paramsKey, paramsPanelVisibleItem->action());

        iWin->addActionItems({
            paramsPanelVisibleItem,
        });
    }

}
