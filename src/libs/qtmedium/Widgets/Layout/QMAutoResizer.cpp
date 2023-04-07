#include "QMAutoResizer.h"

#include <QApplication>
#include <QEvent>
#include <QWidget>

QMAutoResizer::QMAutoResizer(QWidget *parent) : QMAutoResizer(SizeHint, parent) {
}

QMAutoResizer::QMAutoResizer(QMAutoResizer::SizeOption so, QWidget *parent) : QMAutoResizer(so, false, parent) {
}

QMAutoResizer::QMAutoResizer(QMAutoResizer::SizeOption so, bool fixed, QWidget *parent)
    : QMAutoResizer(so, fixed, WidthAndHeight, parent) {
}

QMAutoResizer::QMAutoResizer(QMAutoResizer::SizeOption so, bool fixed, QMAutoResizer::MeasureOption mo, QWidget *parent)
    : QObject(parent), w(parent), so(so), fix(fixed), mo(mo) {
    w->installEventFilter(this);
}

QMAutoResizer::~QMAutoResizer() {
}

bool QMAutoResizer::eventFilter(QObject *obj, QEvent *event) {
    if (obj == w) {
        switch (event->type()) {
            case QEvent::LayoutRequest: {
                auto size = (so == SizeHint) ? w->sizeHint() : w->minimumSizeHint();
                switch (mo) {
                    case Width:
                        fix ? w->setFixedWidth(size.width()) : w->resize(size.width(), w->height());
                        break;
                    case Height:
                        fix ? w->setFixedHeight(size.height()) : w->resize(w->width(), size.height());
                        break;
                    default:
                        fix ? w->setFixedSize(size) : w->resize(size);
                        break;
                }
                QApplication::sendEvent(this, event);
                break;
            }
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}
