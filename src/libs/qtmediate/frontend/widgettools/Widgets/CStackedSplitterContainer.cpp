#include "CStackedSplitterContainer.h"
#include "private/CStackedSplitterContainer_p.h"

CStackedSplitterContainerPrivate::CStackedSplitterContainerPrivate() {
}

CStackedSplitterContainerPrivate::~CStackedSplitterContainerPrivate() {
}

void CStackedSplitterContainerPrivate::init() {
}

CStackedSplitterContainer::CStackedSplitterContainer(QWidget *parent) : CStackedSplitterContainer(*new CStackedSplitterContainerPrivate(), parent) {
}

CStackedSplitterContainer::~CStackedSplitterContainer() {
}

CStackedSplitterContainer::CStackedSplitterContainer(CStackedSplitterContainerPrivate &d, QWidget *parent) : QWidget(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
