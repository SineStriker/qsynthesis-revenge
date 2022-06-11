#include "CCoupleTabDragLabel.h"

CCoupleTabDragLabel::CCoupleTabDragLabel(QWidget *parent)
    : QLabel(parent), currentCard(nullptr), originBar(nullptr), targetBar(nullptr) {
    setFocusPolicy(Qt::StrongFocus);
}

CCoupleTabDragLabel::~CCoupleTabDragLabel() {
}
