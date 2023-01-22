#include "LocalDecorator.h"

#include "CDecorator.h"
#include "QsSystem.h"

#include <QFile>
#include <QScreen>
#include <QWidget>
#include <QWindow>

class LocalDecoratorPrivate {
public:
    LocalDecorator *q_ptr;

    QWidget *w;
    QWindow *winHandle;
    QHash<int, QStringList> qssPaths;

    LocalDecoratorPrivate() {
    }
};

LocalDecorator::LocalDecorator(QWidget *parent)
    : QObject(parent), d_ptr(new LocalDecoratorPrivate()) {
    auto &d = *d_ptr.data();

    d.q_ptr = this;
    d.w = parent;
    d.w->installEventFilter(this);
    d.winHandle = parent->window()->windowHandle(); // If null, get at first show

    Q_SS_NOTIFY(LocalDecorator);
}

LocalDecorator::~LocalDecorator() {
}

void LocalDecorator::reloadScreen(int theme) {
    auto &d = *d_ptr.data();

    auto it = d.qssPaths.find(theme);
    if (it != d.qssPaths.end()) {
        qIDec->applyStyleFiles(
            d.w, d.w->window()->screen()->logicalDotsPerInch() / QsSys::osUnitDpi() * 0.8,
            it.value());
    }
}

void LocalDecorator::addTheme(int theme, const QStringList &paths) {
    d_ptr->qssPaths.insert(theme, paths);
}

void LocalDecorator::removeTheme(int theme) {
    d_ptr->qssPaths.remove(theme);
}

bool LocalDecorator::eventFilter(QObject *obj, QEvent *event) {
    auto &d = *d_ptr.data();
    if (obj == d.w) {
        switch (event->type()) {
            case QEvent::Show: {
                if (!d.winHandle) {
                    d.winHandle = d.w->window()->windowHandle();
                    connect(d.winHandle, &QWindow::screenChanged, this,
                            &LocalDecorator::_q_screenChanged);
                    reloadScreen(qIDec->theme());
                }
                break;
            }
            default:
                break;
        }
    }
    return QObject::eventFilter(obj, event);
}

void LocalDecorator::_q_screenChanged(QScreen *screen) {
    Q_UNUSED(screen);
    reloadScreen(qIDec->theme());
}

void LocalDecorator::_q_deviceRatioChanged(QScreen *screen, double dpi) {
    Q_UNUSED(dpi);
    if (screen == d_ptr->w->screen()) {
        reloadScreen(qIDec->theme());
    }
}

void LocalDecorator::_q_logicalRatioChanged(QScreen *screen, double dpi) {
    Q_UNUSED(dpi);
    if (screen == d_ptr->w->screen()) {
        reloadScreen(qIDec->theme());
    }
}
