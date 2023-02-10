#include "ThemeGuard.h"
#include "CDecorator_p.h"

#include "../CStartInfo.h"

#include <QDebug>
#include <QEvent>
#include <QStyle>
#include <QWindow>

#include <private/qstylesheetstyle_p.h>
#include <private/qwidget_p.h>

// StyleSheet Manager (This class is designed to get the protected member d_ptr in QWidget)

class StyleSheetHacker : public QWidget {
public:
    static void polish(QWidget *w, const QString &stylesheet);
    static void polish(QWidget *w, QWidget *styledWidget);
};

void StyleSheetHacker::polish(QWidget *w, const QString &stylesheet) {
    QWidget w0;
    w0.setStyleSheet(stylesheet);
    polish(w, &w0);
}

void StyleSheetHacker::polish(QWidget *w, QWidget *styledWidget) {
    auto parentStyle = styledWidget->style();

    qDebug() << parentStyle;

    // Ensure it's a QStyleSheetStyle
    if (!parentStyle || qstrcmp(parentStyle->metaObject()->className(), "QStyleSheetStyle")) {
        return;
    }

    QStyle *newStyle = parentStyle;

    // QStyleSheetStyle::ref() is defined inline in the header so we can safely call
    reinterpret_cast<QStyleSheetStyle *>(newStyle)->ref();

    qDebug() << "222"
             << reinterpret_cast<QWidgetPrivate *>(
                    reinterpret_cast<StyleSheetHacker *>(w)->d_ptr.data())
                    ->leftmargin;

    // StyleSheetHacker doesn't have any members even without Q_OBJECT macro so we can safely cast
    reinterpret_cast<QWidgetPrivate *>(reinterpret_cast<StyleSheetHacker *>(w)->d_ptr.data())
        ->setStyle_helper(newStyle, true);

    w->setProperty("styleSheet", styledWidget->styleSheet());
}

// ThemeGuard

ThemeGuard::ThemeGuard(QWidget *w, ThemeSubscriber *g)
    : QObject(qIDec), w(w), needUpdate(false), group(g), screenSet(nullptr),
      queueIterator(CDecoratorPrivate::subscriberUpdateQueue.end()) {
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
            //            qDebug() << "############" << w << it.key()
            //                     << it.key()->style()->metaObject()->className() <<
            //                     "############";
            //            StyleSheetHacker::polish(w, it.key());
            w->setStyleSheet(it.key()->styleSheet());
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
