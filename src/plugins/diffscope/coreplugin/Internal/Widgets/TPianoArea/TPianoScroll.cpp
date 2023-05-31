#include "TPianoScroll.h"
#include "TPianoArea.h"

#include <QEvent>
#include <QResizeEvent>
#include <QScrollBar>

TPianoScroll::TPianoScroll(QWidget *parent) : QsApi::NoneScrollArea(parent) {
    m_area = nullptr;
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
}

TPianoScroll::~TPianoScroll() {
}

Core::IPianoKeyWidget *TPianoScroll::area() const {
    return m_area;
}

void TPianoScroll::setArea(Core::IPianoKeyWidget *area) {
    if (m_area) {
        m_area->removeEventFilter(this);
    }
    m_area = area;
    m_area->installEventFilter(this);

    setWidget(area);
    updateGeometry();
}

QSize TPianoScroll::sizeHint() const {
    QSize sz = NoneScrollArea::sizeHint();
    return m_area ? QSize(m_area->width(), sz.height()) : sz;
}

QSize TPianoScroll::minimumSizeHint() const {
    QSize sz = NoneScrollArea::minimumSizeHint();
    return m_area ? QSize(m_area->width(), sz.height()) : sz;
}

bool TPianoScroll::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::Resize) {
        auto e = static_cast<QResizeEvent *>(event);
        if (e->oldSize().width() != e->size().width()) {
            updateGeometry();
        }
    }
    return QObject::eventFilter(obj, event);
}
