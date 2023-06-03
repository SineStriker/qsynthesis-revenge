#include "TrackPanel.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QVariant>

namespace Core::Internal {

    TrackPanel::TrackPanel(QWidget *parent) : QFrame(parent) {
        setProperty("dock-panel", true);

        auto label = new QLabel("ACE FUCK YOUR ASSHOLE!!!");
        label->setStyleSheet(R"(

color: rgba(255, 255, 255, 75%);
font-size: 72px;
background-color: rgba(255, 0, 0, 25%);

)");

        auto layout = new QVBoxLayout();
        layout->setMargin(0);
        layout->setSpacing(0);
        layout->addWidget(label);
        setLayout(layout);
    }

    TrackPanel::~TrackPanel() {
    }

}