#include "CDockPanelSplitter.h"

CDockPanelSplitter::CDockPanelSplitter(QWidget *parent) : QSplitter(parent) {
}

CDockPanelSplitter::CDockPanelSplitter(Qt::Orientation orient, QWidget *parent)
    : QSplitter(orient, parent) {
}

CDockPanelSplitter::~CDockPanelSplitter() {
}
