#include "CCoupleTabSplitter.h"

CCoupleTabSplitter::CCoupleTabSplitter(QWidget *parent) : QSplitter(parent) {
}

CCoupleTabSplitter::CCoupleTabSplitter(Qt::Orientation orient, QWidget *parent)
    : QSplitter(orient, parent) {
}

CCoupleTabSplitter::~CCoupleTabSplitter() {
}
