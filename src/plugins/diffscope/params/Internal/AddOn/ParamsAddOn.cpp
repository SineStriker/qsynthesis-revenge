#include "ParamsAddOn.h"

#include <QMDecoratorV2.h>

#include <coreplugin/Window/IProjectWindow.h>

namespace Params::Internal {

    using namespace Core;

    static const char *paramsKey = "edit.paramsPanel";

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

        auto pianoRoll = iWin->pianoRoll();

        // Add phoneme panel
        paramsPanel = new Internal::ParamsPanel();
        paramsButton = pianoRoll->addFloatingPanel(paramsKey, paramsPanel, nullptr);

        connect(pianoRoll, &PianoRoll::floatingPanelStateChanged, this,
                [iWin](const QString &id, PianoRoll::FloatingPanelState state) {
                    if (id == paramsKey) {
                        iWin->setGlobalAttribute(paramsKey, state != PianoRoll::Hidden);
                    }
                });

        connect(iWin, &IWindow::globalAttributeChanged, this,
                [pianoRoll](const QString &id, const QVariant &var, const QVariant &orgVar) {
                    if (id != paramsKey) {
                        return;
                    }
                    if (var.toBool()) {
                        if (pianoRoll->floatingPanelState(paramsKey) == PianoRoll::Hidden) {
                            pianoRoll->setFloatingPanelState(paramsKey, PianoRoll::Normal);
                        }
                    } else {
                        if (pianoRoll->floatingPanelState(paramsKey) != PianoRoll::Hidden) {
                            pianoRoll->setFloatingPanelState(paramsKey, PianoRoll::Hidden);
                        }
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