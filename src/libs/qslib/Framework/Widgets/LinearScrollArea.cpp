#include "LinearScrollArea.h"
#include "Widgets/CScrollBar.h"

LinearScrollArea::LinearScrollArea(Qt::Orientation ori, QWidget *parent) : CScrollArea(parent), m_orient(ori) {
    if (ori == Qt::Horizontal) {
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        verticalScrollBar()->setEnabled(false);
    } else {
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        horizontalScrollBar()->setEnabled(false);
    }
}

LinearScrollArea::~LinearScrollArea() {
}

Qt::Orientation LinearScrollArea::orient() const {
    return m_orient;
}

void LinearScrollArea::resizeEvent(QResizeEvent *event) {
    QWidget *w = widget();
    if (w) {
        if (m_orient == Qt::Horizontal) {
            if (w->height() != viewport()->height()) {
                w->setFixedHeight(viewport()->height());
            }
        } else {
            if (w->width() != viewport()->width()) {
                w->setFixedWidth(viewport()->width());
            }
        }
    }
    QScrollArea::resizeEvent(event);
}
