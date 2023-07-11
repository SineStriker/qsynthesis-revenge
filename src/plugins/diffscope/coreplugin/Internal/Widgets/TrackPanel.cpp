#include "TrackPanel.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QVariant>

namespace Core::Internal {

    TrackPanel::TrackPanel(IProjectWindow *iWin, QWidget *parent) : ISidePanel(iWin, parent) {
        setProperty("dock-panel", true);

        setWindowTitle("Tracks");

        auto label = new QLabel("This is the track panel!!!");
        label->setStyleSheet(R"(

color: rgba(255, 255, 255, 75%);
font-size: 72px;
background-color: rgba(255, 0, 0, 25%);

)");

        setCentralWidget(label);
    }

    TrackPanel::~TrackPanel() {
    }

    void TrackPanel::initialize() {
    }

    void TrackPanel::extensionInitialized() {
    }

}