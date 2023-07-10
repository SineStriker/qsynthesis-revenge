#include "FloatingWindowHelper.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QHoverEvent>
#include <QWidget>

FloatingWindowHelper::FloatingWindowHelper(QWidget *w, QObject *parent) : QObject(parent), w(w) {
    m_resizeMargins = {5, 5, 5, 5};

    m_floating = false;
    m_windowFlags = 0;
    m_pressed = false;
    m_pressedArea = None;

    // Initialize array
    for (int i = 0; i < SizeOfEdgeAndCorner; ++i) {
        m_pressedRect[i] = {0, 0, 0, 0};
    }
    w->setAttribute(Qt::WA_Hover);
}

FloatingWindowHelper::~FloatingWindowHelper() {
}

void FloatingWindowHelper::addTitleBarWidget(QWidget *w) {
    m_titleBarWidgets.insert(w);
    connect(w, &QObject::destroyed, this, &FloatingWindowHelper::_q_titleBarWidgetDestroyed);
}

void FloatingWindowHelper::removeTitleBarWidget(QWidget *w) {
    m_titleBarWidgets.remove(w);
    disconnect(w, &QObject::destroyed, this, &FloatingWindowHelper::_q_titleBarWidgetDestroyed);
}

bool FloatingWindowHelper::floating() const {
    return m_floating;
}

void FloatingWindowHelper::setFloating(bool floating, Qt::WindowFlags flags) {
    if (m_floating == floating)
        return;

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

QMargins FloatingWindowHelper::resizeMargins() const {
    return m_resizeMargins;
}

void FloatingWindowHelper::setResizeMargins(const QMargins &resizeMargins) {
    m_resizeMargins = resizeMargins;
}

bool FloatingWindowHelper::eventFilter(QObject *obj, QEvent *event) {
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
                    auto me = *e;
                    me.setLocalPos(w->mapFromGlobal(e->globalPos()));
                    if (dummyEventFilter(obj, &me)) {
                        return true;
                    }
                }
            }
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}

bool FloatingWindowHelper::dummyEventFilter(QObject *obj, QEvent *event) {
    switch (event->type()) {
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
            } else if (m_pressedRect[TopLeft].contains(pos) ||
                       m_pressedRect[BottomRight].contains(pos)) {
                w->setCursor(Qt::SizeFDiagCursor);
            } else if (m_pressedRect[TopRight].contains(pos) ||
                       m_pressedRect[BottomLeft].contains(pos)) {
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
                if (widget && (widget == w || m_titleBarWidgets.contains(widget))) {
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

void FloatingWindowHelper::_q_titleBarWidgetDestroyed() {
    removeTitleBarWidget(static_cast<QWidget *>(sender()));
}
