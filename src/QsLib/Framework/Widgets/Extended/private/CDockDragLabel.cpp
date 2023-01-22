#include "CDockDragLabel.h"

CDockDragLabel::CDockDragLabel(QWidget *parent)
    : QLabel(parent), currentCard(nullptr), originBar(nullptr), targetBar(nullptr) {
    setFocusPolicy(Qt::StrongFocus);
}

CDockDragLabel::~CDockDragLabel() {
}
