#include "ThemeSubscriber.h"
#include "CDecorator_p.h"

#include <QEvent>
#include <QWindow>

ThemeSubscriber::ThemeSubscriber(QWidget *w, CDecorator *dec, CDecoratorPrivate *decp)
    : QObject(dec), dec(dec), decp(decp), w(w) {
    winHandle = w->windowHandle();
    w->installEventFilter(this);

    connect(dec, &CDecorator::deviceRatioChanged, this, &ThemeSubscriber::_q_deviceRatioChanged);
    connect(dec, &CDecorator::logicalRatioChanged, this, &ThemeSubscriber::_q_logicalRatioChanged);
}

ThemeSubscriber::~ThemeSubscriber() {
}

void ThemeSubscriber::updateScreen() {
    for (const auto &ns : qAsConst(namespaces)) {
        auto it2 = decp->themeNamespaces.find(ns);
        if (it2 == decp->themeNamespaces.end()) {
            continue;
        }

        auto it3 = decp->themes.find(*it2);
        if (it3 == decp->themes.end()) {
            continue;
        }

        auto it4 = it3->configs.find(dec->locale());
        if (it4 == it3->configs.end()) {
            continue;
        }

        ThemeConfig conf;
        if (!conf.load(it3.))

        for (const auto &temp : qAsConst(it3->templates)) {
            QString stylesheet = temp.parse()
        }
    }
}

bool ThemeSubscriber::eventFilter(QObject *obj, QEvent *event) {
    if (obj == w) {
        switch (event->type()) {
            case QEvent::Show: {
                if (!winHandle) {
                    winHandle = w->window()->windowHandle();
                    connect(winHandle, &QWindow::screenChanged, this,
                            &ThemeSubscriber::_q_screenChanged);
                    // reloadScreen(qIDec->theme());
                }
                break;
            }
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}

void ThemeSubscriber::_q_screenChanged(QScreen *screen) {
}

void ThemeSubscriber::_q_deviceRatioChanged(QScreen *screen, double dpi) {
    if (screen == w->screen()) {
        updateScreen();
    }
}

void ThemeSubscriber::_q_logicalRatioChanged(QScreen *screen, double dpi) {
    if (screen == w->screen()) {
        updateScreen();
    }
}