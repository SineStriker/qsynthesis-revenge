#include "IPianoKeyWidget.h"

namespace Core {

    IPianoKeyWidgetFactory::~IPianoKeyWidgetFactory() {
    }

    IPianoKeyWidget::IPianoKeyWidget(IProjectWindow *iWin, QWidget *parent)
        : QFrame(parent), IPianoRollComponent(iWin) {
    }

    IPianoKeyWidget::~IPianoKeyWidget() {
    }

}