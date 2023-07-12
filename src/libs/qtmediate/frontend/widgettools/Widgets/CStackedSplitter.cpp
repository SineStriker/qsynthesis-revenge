#include "CStackedSplitter.h"
#include "private/CStackedSplitter_p.h"

CStackedSplitterPrivate::CStackedSplitterPrivate() {
}

CStackedSplitterPrivate::~CStackedSplitterPrivate() {
}

void CStackedSplitterPrivate::init() {
}

CStackedSplitter::CStackedSplitter(QWidget *parent) : CStackedSplitter(*new CStackedSplitterPrivate(), parent) {
}

CStackedSplitter::~CStackedSplitter() {
}

CStackedSplitter::CStackedSplitter(CStackedSplitterPrivate &d, QWidget *parent) : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
