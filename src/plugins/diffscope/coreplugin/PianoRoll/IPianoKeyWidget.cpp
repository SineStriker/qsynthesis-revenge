#include "IPianoKeyWidget.h"

namespace Core {

    IPianoKeyWidgetFactory::~IPianoKeyWidgetFactory() {
    }

    IPianoKeyWidget::IPianoKeyWidget(QWidget *parent) : QFrame(parent) {
    }

    IPianoKeyWidget::~IPianoKeyWidget() {
    }

}