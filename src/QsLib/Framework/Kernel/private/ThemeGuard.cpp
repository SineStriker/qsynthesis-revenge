#include "ThemeGuard.h"
#include "CDecorator_p.h"

#include <QDebug>
#include <QEvent>
#include <QWindow>

ThemeGuard::ThemeGuard(QWidget *w, ThemeSubscriber *g)
    : QObject(qIDec), w(w), needUpdate(false), group(g), screenSet(nullptr) {
    winHandle = w->windowHandle();
    w->installEventFilter(this);
}

ThemeGuard::~ThemeGuard() {
}

void ThemeGuard::updateScreen() {
    if (!winHandle) {
        needUpdate = true;
        return;
    }

    if (!screenSet->dirty) {
        const auto &ws = screenSet->widgets;
        auto it = ws.begin();
        while (it != ws.end() && it.value() == this) {
            it++;
        }
        if (it != ws.end()) {
            w->setStyle(it.key()->style());
            return;
        }
    }
    screenSet->dirty = false;

    QString stylesheet;
    for (const auto &tp : qAsConst(group->templates)) {
        if (tp->data.isNull()) {
            continue;
        }
        const QString &s = tp->getAndCache(screenSet->screen);
        if (s.isEmpty()) {
            continue;
        }

        stylesheet.append(s + "\n\n");
    }

    w->setStyleSheet(stylesheet);
}

bool ThemeGuard::eventFilter(QObject *obj, QEvent *event) {
    if (obj == w) {
        switch (event->type()) {
            case QEvent::Show: {
                if (!winHandle) {
                    winHandle = w->window()->windowHandle();
                    connect(winHandle, &QWindow::screenChanged, this,
                            &ThemeGuard::_q_screenChanged);
                    if (needUpdate) {
                        needUpdate = false;
                        updateScreen();
                    }
                }
                break;
            }
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}

void ThemeGuard::_q_screenChanged(QScreen *screen) {
    group->switchScreen(this);
    updateScreen();
}
