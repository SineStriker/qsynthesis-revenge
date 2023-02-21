#include "CScrollArea.h"
#include "CScrollBar.h"

CScrollArea::CScrollArea(QWidget *parent) : QScrollArea(parent) {
    CScrollBar::replaceScrollBars(this);
}

CScrollArea::~CScrollArea() {
}

void CScrollArea::setValueX(int value) {
    horizontalScrollBar()->setValue(value);
}

int CScrollArea::valueX() const {
    return horizontalScrollBar()->value();
}

void CScrollArea::setValueY(int value) {
    verticalScrollBar()->setValue(value);
}

int CScrollArea::valueY() const {
    return verticalScrollBar()->value();
}
