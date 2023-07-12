#include "QMFloatingWindowHelper.h"
#include "QMFloatingWindowHelper_p.h"

#include <QApplication>
#include <QMouseEvent>
#include <QWidget>

static bool isDescendantOfWidget(QWidget *widget, QWidget *parent) {
    auto upper = parent->parentWidget();
    while (widget && widget != upper) {
        if (widget == parent)
            return true;
        widget = widget->parentWidget();
    }
    return false;
}

QMFloatingWindowHelperPrivate::QMFloatingWindowHelperPrivate(QWidget *w, QMFloatingWindowHelper *q) : q(q), w(w) {
    m_resizeMargins = {5, 5, 5, 5};

    m_floating = false;
    m_windowFlags = 0;
    m_pressed = false;
    m_pressedArea = None;

    // Initialize array
    for (auto &r : m_pressedRect) {
        r = {0, 0, 0, 0};
    }

    w->setAttribute(Qt::WA_Hover);
}

QMFloatingWindowHelperPrivate::~QMFloatingWindowHelperPrivate() {
}

void QMFloatingWindowHelperPrivate::setFloating_helper(bool floating, Qt::WindowFlags flags) {
    m_floating = floating;

    if (floating) {
        m_windowFlags = w->windowFlags();
        w->setWindowFlags(flags | Qt::FramelessWindowHint);
        qApp->installEventFilter(this);
    } else {
        qApp->removeEventFilter(this);
        w->setWindowFlags(static_cast<Qt::WindowFlags>(m_windowFlags));
        m_windowFlags = 0;
    }
}

bool QMFloatingWindowHelperPrivate::dummyEventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
        case QEvent::Show:
        case QEvent::Resize: {
            // Re-calc anchor areas
            int width = w->width();
            int height = w->height();

            int left = m_resizeMargins.left();
            int right = m_resizeMargins.right();
            int top = m_resizeMargins.top();
            int bottom = m_resizeMargins.bottom();

            m_pressedRect[Left] = QRect(0, top, left, height - top - bottom);
            m_pressedRect[Right] = QRect(width - right, top, right, height - top - bottom);
            m_pressedRect[Top] = QRect(left, 0, width - left - right, top);
            m_pressedRect[Bottom] = QRect(left, height - bottom, width - left - right, bottom);

            m_pressedRect[TopLeft] = QRect(0, 0, left, top);
            m_pressedRect[TopRight] = QRect(width - right, 0, right, top);
            m_pressedRect[BottomLeft] = QRect(0, height - bottom, left, bottom);
            m_pressedRect[BottomRight] = QRect(width - right, height - bottom, right, bottom);

            break;
        }
        case QEvent::HoverMove: {
            auto e = static_cast<QHoverEvent *>(event);
            QPoint pos = e->pos();

            // Set cursor
            if (m_pressedRect[Left].contains(pos) || m_pressedRect[Right].contains(pos)) {
                w->setCursor(Qt::SizeHorCursor);
            } else if (m_pressedRect[Top].contains(pos) || m_pressedRect[Bottom].contains(pos)) {
                w->setCursor(Qt::SizeVerCursor);
            } else if (m_pressedRect[TopLeft].contains(pos) || m_pressedRect[BottomRight].contains(pos)) {
                w->setCursor(Qt::SizeFDiagCursor);
            } else if (m_pressedRect[TopRight].contains(pos) || m_pressedRect[BottomLeft].contains(pos)) {
                w->setCursor(Qt::SizeBDiagCursor);
            } else {
                w->setCursor(Qt::ArrowCursor);
            }

            // Calc the movement by mouse pos
            int offsetX = pos.x() - m_pressedPos.x();
            int offsetY = pos.y() - m_pressedPos.y();

            int rectX = m_orgGeometry.x();
            int rectY = m_orgGeometry.y();
            int rectW = m_orgGeometry.width();
            int rectH = m_orgGeometry.height();

            int minW = w->minimumWidth();
            int minH = w->minimumHeight();

            switch (m_pressedArea) {
                case Left: {
                    int resizeW = w->width() - offsetX;
                    if (minW <= resizeW) {
                        w->setGeometry(w->x() + offsetX, rectY, resizeW, rectH);
                    }
                    break;
                }
                case Right: {
                    w->setGeometry(rectX, rectY, rectW + offsetX, rectH);
                    break;
                }
                case Top: {
                    int resizeH = w->height() - offsetY;
                    if (minH <= resizeH) {
                        w->setGeometry(rectX, w->y() + offsetY, rectW, resizeH);
                    }
                    break;
                }
                case Bottom: {
                    w->setGeometry(rectX, rectY, rectW, rectH + offsetY);
                    break;
                }
                case TopLeft: {
                    int resizeW = w->width() - offsetX;
                    int resizeH = w->height() - offsetY;
                    if (minW <= resizeW) {
                        w->setGeometry(w->x() + offsetX, w->y(), resizeW, resizeH);
                    }
                    if (minH <= resizeH) {
                        w->setGeometry(w->x(), w->y() + offsetY, resizeW, resizeH);
                    }
                    break;
                }
                case TopRight: {
                    int resizeW = rectW + offsetX;
                    int resizeH = w->height() - offsetY;
                    if (minH <= resizeH) {
                        w->setGeometry(w->x(), w->y() + offsetY, resizeW, resizeH);
                    }
                    break;
                }
                case BottomLeft: {
                    int resizeW = w->width() - offsetX;
                    int resizeH = rectH + offsetY;
                    if (minW <= resizeW) {
                        w->setGeometry(w->x() + offsetX, w->y(), resizeW, resizeH);
                    }
                    if (minH <= resizeH) {
                        w->setGeometry(w->x(), w->y(), resizeW, resizeH);
                    }
                    break;
                }
                case BottomRight: {
                    int resizeW = rectW + offsetX;
                    int resizeH = rectH + offsetY;
                    w->setGeometry(w->x(), w->y(), resizeW, resizeH);
                    break;
                }
                default: {
                    if (m_pressed) {
                        // Execute stretch or move
                        w->move(w->x() + offsetX, w->y() + offsetY);
                    }
                    break;
                }
            }
            break;
        }
        case QEvent::MouseButtonPress: {
            // Record mouse press coordinates
            auto e = (QMouseEvent *) event;
            m_pressedPos = e->pos();
            m_orgGeometry = w->geometry();

            bool pressed = false;
            for (int i = 0; i < SizeOfEdgeAndCorner; ++i) {
                if (m_pressedRect[i].contains(m_pressedPos)) {
                    m_pressedArea = static_cast<EdgeAndCorner>(i);
                    pressed = true;
                    break;
                }
            }

            if (!pressed) {
                auto widget = qobject_cast<QWidget *>(obj);
                if (widget && (widget == w || QMFloatingWindowHelper::isWidgetHitTestVisible(w))) {
                    m_pressed = true;
                    pressed = true;
                }
            }

            if (pressed) {
                return true;
            }
            break;
        }
        case QEvent::Leave:
        case QEvent::MouseButtonRelease: {
            // Restore all
            w->setCursor(Qt::ArrowCursor);
            bool pressed = m_pressed || m_pressedArea != None;

            m_pressed = false;
            m_pressedArea = None;

            if (pressed) {
                return true;
            }
            break;
        }
        default:
            break;
    }
    return false;
}

bool QMFloatingWindowHelperPrivate::eventFilter(QObject *obj, QEvent *event) {
    if (obj == w) {
        if (dummyEventFilter(obj, event)) {
            return true;
        }
    } else {
        switch (event->type()) {
            case QEvent::MouseButtonPress:
            case QEvent::MouseButtonRelease: {
                if (obj->isWidgetType()) {
                    auto e = static_cast<QMouseEvent *>(event);
                    auto widget = qobject_cast<QWidget *>(obj);
                    if (widget && isDescendantOfWidget(widget, w)) {
                        auto me = *e;
                        me.setLocalPos(w->mapFromGlobal(e->globalPos()));
                        if (dummyEventFilter(obj, &me)) {
                            return true;
                        }
                    }
                }
            }
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}

QMFloatingWindowHelper::QMFloatingWindowHelper(QWidget *w, QObject *parent)
    : QObject(parent), d(new QMFloatingWindowHelperPrivate(w, this)) {
}

QMFloatingWindowHelper::~QMFloatingWindowHelper() {
    delete d;
}

bool QMFloatingWindowHelper::floating() const {
    return d->m_floating;
}

void QMFloatingWindowHelper::setFloating(bool floating, Qt::WindowFlags flags) {
    if (d->m_floating == floating)
        return;
    d->setFloating_helper(floating, flags);
}

QMargins QMFloatingWindowHelper::resizeMargins() const {
    return d->m_resizeMargins;
}

void QMFloatingWindowHelper::setResizeMargins(const QMargins &resizeMargins) {
    d->m_resizeMargins = resizeMargins;
}

static const char KEY_NAME_HIT_TEST_VISIBLE[] = "qtm_hit_test_visible";

bool QMFloatingWindowHelper::isWidgetHitTestVisible(QWidget *w) {
    return w->property(KEY_NAME_HIT_TEST_VISIBLE).toBool();
}

void QMFloatingWindowHelper::setWidgetHitTestVisible(QWidget *w, bool value) {
    w->setProperty(KEY_NAME_HIT_TEST_VISIBLE, value);
}
