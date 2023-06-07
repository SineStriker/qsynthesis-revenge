#include "PianoKeyContainer.h"

#include <QEvent>
#include <QResizeEvent>
#include <QScrollBar>

namespace Core::Internal {

    PianoKeyContainer::PianoKeyContainer(QWidget *parent) : QsApi::NoneScrollArea(parent) {
        m_area = nullptr;
        setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    }

    PianoKeyContainer::~PianoKeyContainer() {
    }

    Core::IPianoKeyWidget *PianoKeyContainer::area() const {
        return m_area;
    }

    void PianoKeyContainer::setArea(Core::IPianoKeyWidget *area) {
        if (m_area) {
            m_area->removeEventFilter(this);
        }
        m_area = area;
        if (m_area) {
            m_area->installEventFilter(this);
        }

        setWidget(area);
        updateGeometry();
    }

    QSize PianoKeyContainer::sizeHint() const {
        QSize sz = NoneScrollArea::sizeHint();
        return m_area ? QSize(m_area->width(), sz.height()) : sz;
    }

    QSize PianoKeyContainer::minimumSizeHint() const {
        QSize sz = NoneScrollArea::minimumSizeHint();
        return m_area ? QSize(m_area->width(), sz.height()) : sz;
    }

    bool PianoKeyContainer::eventFilter(QObject *obj, QEvent *event) {
        if (event->type() == QEvent::Resize) {
            auto e = static_cast<QResizeEvent *>(event);
            if (e->oldSize().width() != e->size().width()) {
                updateGeometry();
            }
        }
        return QObject::eventFilter(obj, event);
    }

}