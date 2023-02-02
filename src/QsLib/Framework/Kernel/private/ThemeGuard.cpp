#include "ThemeGuard.h"
#include "CDecorator_p.h"

#include "../CStartInfo.h"

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

bool ThemeGuard::updateScreen() {
    if (!winHandle || qIStup->isAboutToQuit()) {
        needUpdate = true;
        return false;
    }

    if (!screenSet->dirty) {
        const auto &ws = screenSet->widgets;
        auto it = ws.begin();
        while (it != ws.end() && it.value() == this) {
            it++;
        }
        if (it != ws.end()) {
            w->setStyle(it.key()->style());
            return true;
        }
    }
    screenSet->dirty = false;

    QString stylesheet;
    for (const auto &key : qAsConst(group->keySeq)) {
        auto &tp = group->templates[key];
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
    return true;
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
                        CDecoratorPrivate::subscriberUpdateEnqueue(nullptr);
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

    qDebug() << "Window" << this << "screen changed to" << screen;

    CDecoratorPrivate::subscriberUpdateEnqueue(this);
}
