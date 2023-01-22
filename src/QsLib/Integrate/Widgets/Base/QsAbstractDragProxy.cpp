#include "QsAbstractDragProxy.h"

QsAbstractDragProxy::QsAbstractDragProxy(QObject *parent) : QObject(parent) {
    m_activeArea = nullptr;
}

QsAbstractDragProxy::~QsAbstractDragProxy() {
}

QWidget *QsAbstractDragProxy::activeArea() const {
    return m_activeArea;
}

void QsAbstractDragProxy::setActiveArea(QWidget *activeArea) {
    m_activeArea = activeArea;
}
