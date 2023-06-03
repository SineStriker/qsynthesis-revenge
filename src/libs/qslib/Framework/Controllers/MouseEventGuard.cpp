#include "MouseEventGuard.h"

#include <QApplication>
#include <QDebug>
#include <QMetaObject>
#include <QMouseEvent>

namespace QsApi {

    class MouseEventGuardPrivate : public QObject {
    public:
        Qt::MouseButton b;
        QByteArray slot;
        QWidget *w;
        bool pressed;

        MouseEventGuardPrivate(Qt::MouseButton b, const char *slot, QWidget *w)
            : b(b), slot(slot), w(w), pressed(false) {
            w->installEventFilter(this);
        }

        void call(QMouseEvent *e) {
            QMetaObject::invokeMethod(w, slot, Qt::DirectConnection, Q_ARG(QMouseEvent *, e));
        }

        void start(QMouseEvent *e) {
            pressed = true;
            qApp->installEventFilter(this);
            call(e);
        }

        void release(QMouseEvent *e) {
            pressed = false;
            qApp->removeEventFilter(this);
            call(e);
        }

        void abandon(QMouseEvent *e) {
            pressed = false;
            qApp->removeEventFilter(this);
            QMouseEvent e2(QEvent::MouseButtonRelease, w->mapFromGlobal(e->globalPos()), b,
                           Qt::NoButton, e->modifiers());
            call(&e2);
        }

        bool eventFilter(QObject *obj, QEvent *event) override {
            switch (event->type()) {
                case QEvent::MouseButtonPress: {
                    auto e = static_cast<QMouseEvent *>(event);
                    if (obj == w) {
                        if (e->button() == b) {
                            if (!pressed) {
                                start(e);
                            } else {
                                // Same button pressed twice without release
                                abandon(e);
                            }
                        }
                    } else {
                        // Unexpected press encountered
                        abandon(e);
                    }
                    break;
                }
                case QEvent::MouseMove: {
                    auto e = static_cast<QMouseEvent *>(event);
                    if (e->buttons() == Qt::NoButton) {
                        // Unexpected move encountered
                        abandon(e);
                    } else if ((e->buttons() & b) && pressed) {
                        call(e);
                    }
                    break;
                }
                case QEvent::MouseButtonRelease: {
                    auto e = static_cast<QMouseEvent *>(event);
                    if (pressed) {
                        abandon(e);
                    }
                    break;
                }
                default:
                    break;
            }

            return QObject::eventFilter(obj, event);
        }
    };

    MouseEventGuard::MouseEventGuard(Qt::MouseButton button, const char *slot, QWidget *parent)
        : QObject(parent), d(new MouseEventGuardPrivate(button, slot, parent)) {
    }

    MouseEventGuard::~MouseEventGuard() {
        delete d;
    }


} // namespace QsApi
