#include "ThemeGuard.h"
#include "CDecorator_p.h"

#include <QEvent>
#include <QWindow>

ThemeGuard::ThemeGuard(QWidget *w, CDecorator *dec, CDecoratorPrivate *decp, ThemeSubscriber *g)
    : QObject(dec), dec(dec), decp(decp), w(w), needUpdate(false), group(g) {
    winHandle = w->windowHandle();
    w->installEventFilter(this);

    connect(dec, &CDecorator::deviceRatioChanged, this, &ThemeGuard::_q_deviceRatioChanged);
    connect(dec, &CDecorator::logicalRatioChanged, this, &ThemeGuard::_q_logicalRatioChanged);
}

ThemeGuard::~ThemeGuard() {
}

void ThemeGuard::updateScreen() {
    if (!winHandle) {
        needUpdate = true;
        return;
    }

    if (!group->dirty) {
        auto it = group->widgets.begin();
        while (it.value() == this) {
            it++;
        }
        w->setStyle(it.key()->style());
        return;
    }
    group->dirty = false;

    QString stylesheet;
    for (const auto &ref : qAsConst(group->templates)) {
        if (ref->data.isNull()) {
            continue;
        }
        auto it = ref->stylesheetCaches.find(decp->theme);
        if (it == ref->stylesheetCaches.end()) {
            continue;
        }

        const QString &s = it.value();
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
    updateScreen();
}

void ThemeGuard::_q_deviceRatioChanged(QScreen *screen, double dpi) {
    if (screen == w->screen()) {
        updateScreen();
    }
}

void ThemeGuard::_q_logicalRatioChanged(QScreen *screen, double dpi) {
    if (screen == w->screen()) {
        updateScreen();
    }
}