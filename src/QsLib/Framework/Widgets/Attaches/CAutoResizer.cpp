#include "CAutoResizer.h"

#include <QEvent>
#include <QWidget>

CAutoResizer::CAutoResizer(QWidget *parent) : CAutoResizer(SizeHint, parent) {
}

CAutoResizer::CAutoResizer(CAutoResizer::SizeOption so, QWidget *parent)
    : CAutoResizer(so, false, parent) {
}

CAutoResizer::CAutoResizer(CAutoResizer::SizeOption so, bool fixed, QWidget *parent)
    : CAutoResizer(so, fixed, WidthAndHeight, parent) {
}

CAutoResizer::CAutoResizer(CAutoResizer::SizeOption so, bool fixed, CAutoResizer::MeasureOption mo,
                           QWidget *parent)
    : QObject(parent), w(parent), so(so), fix(fixed), mo(mo) {
    w->installEventFilter(this);
}

CAutoResizer::~CAutoResizer() {
}

bool CAutoResizer::eventFilter(QObject *obj, QEvent *event) {
    if (obj == w) {
        switch (event->type()) {
            case QEvent::LayoutRequest: {
                auto size = (so == SizeHint) ? w->sizeHint() : w->minimumSizeHint();
                switch (mo) {
                    case Width:
                        fix ? w->setFixedWidth(size.width()) : w->resize(size.width(), w->height());
                        break;
                    case Height:
                        fix ? w->setFixedHeight(size.height())
                            : w->resize(w->width(), size.height());
                        break;
                    default:
                        fix ? w->setFixedSize(size) : w->resize(size);
                        break;
                }
                break;
            }
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}
