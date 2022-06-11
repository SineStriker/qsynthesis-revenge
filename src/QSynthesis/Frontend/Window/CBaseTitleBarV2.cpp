#include "CBaseTitleBarV2.h"

CBaseTitleBarV2::CBaseTitleBarV2(QWidget *parent) : ANativeTitleBar(parent) {
    m_titleColor = Qt::black;
}

CBaseTitleBarV2::~CBaseTitleBarV2() {
}

QColor CBaseTitleBarV2::titleColor() const {
    return m_titleColor;
}

void CBaseTitleBarV2::setTitleColor(const QColor &titleColor) {
    m_titleColor = titleColor;
}
