#include "BaseDragProxy.h"

BaseDragProxy::BaseDragProxy(QObject *parent) : QObject(parent) {
    m_activeArea = nullptr;
}

BaseDragProxy::~BaseDragProxy() {
}

QWidget *BaseDragProxy::activeArea() const {
    return m_activeArea;
}

void BaseDragProxy::setActiveArea(QWidget *activeArea) {
    m_activeArea = activeArea;
}
