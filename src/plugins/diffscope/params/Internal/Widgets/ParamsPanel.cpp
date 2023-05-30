#include "ParamsPanel.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QVariant>

namespace Params::Internal {

    ParamsPanel::ParamsPanel(QWidget *parent) : QFrame(parent) {
        setProperty("floating-panel", true);
        auto label = new QLabel("ACE IS SO GREAT!!!");
        label->setStyleSheet(R"(

color: rgba(255, 255, 255, 75%);
font-size: 72px;

)");

        auto layout = new QVBoxLayout();
        layout->addWidget(label);
        setLayout(layout);
    }

    ParamsPanel::~ParamsPanel() {
    }

}