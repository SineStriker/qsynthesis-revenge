#include "PhonemePanel.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QVariant>

namespace Core::Internal {

    PhonemePanel::PhonemePanel(IProjectWindow *iWin, QWidget *parent) : QFrame(parent), IPianoRollComponent(iWin) {
        setProperty("floating-panel", true);

        auto label = new QLabel("This is the phoneme panel!!!");
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

    PhonemePanel::~PhonemePanel() {
    }

    void PhonemePanel::initialize() {
    }

    void PhonemePanel::extensionInitialized() {
    }

}