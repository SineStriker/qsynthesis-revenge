#include "ParamsPanel.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QVariant>

namespace Params::Internal {

    ParamsPanel::ParamsPanel(Core::IProjectWindow *iWin, QWidget *parent)
        : QFrame(parent), Core::IPianoRollComponent(iWin) {
        setProperty("floating-panel", true);
        auto label = new QLabel("This is the params panel!!!");
        label->setStyleSheet(R"(

color: rgba(255, 255, 255, 75%);
font-size: 72px;

)");

        auto layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(label);
        setLayout(layout);
    }

    ParamsPanel::~ParamsPanel() {
    }

    void ParamsPanel::initialize() {
    }

    void ParamsPanel::extensionInitialized() {
    }

}