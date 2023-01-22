#include "CDecorator_p.h"

#include <QJsonDocument>
#include <QJsonObject>

#include <QApplication>
#include <QWidget>
#include <QWindow>

static const char Theme_Variable_Hint_Size[] = "size";
static const char Theme_Variable_Hint_Color[] = "color";

static const char Theme_Config_Key_Sizes[] = "sizes";
static const char Theme_Config_Key_Colors[] = "colors";
static const char Theme_Config_Key_Separator[] = ".";

static const char Theme_RegExp_Pattern[] = "\\\"\\{\\{.*\\}\\}\\\"";
static const char Theme_RegExp_Separator[] = "|";
static const int Theme_RegExp_Pattern_Left_Length = 3;
static const int Theme_RegExp_Pattern_Right_Length = 3;

CDecoratorFilter::CDecoratorFilter(QWidget *w, CDecorator *dec, CDecoratorPrivate *decp)
    : QObject(dec), dec(dec), decp(decp), w(w) {
    winHandle = w->windowHandle();
    w->installEventFilter(this);

    connect(dec, &CDecorator::deviceRatioChanged, this, &CDecoratorFilter::_q_deviceRatioChanged);
    connect(dec, &CDecorator::logicalRatioChanged, this, &CDecoratorFilter::_q_logicalRatioChanged);
}

CDecoratorFilter::~CDecoratorFilter() {
}

bool CDecoratorFilter::eventFilter(QObject *obj, QEvent *event) {
    if (obj == w) {
        switch (event->type()) {
            case QEvent::Show: {
                if (!winHandle) {
                    winHandle = w->window()->windowHandle();
                    connect(winHandle, &QWindow::screenChanged, this,
                            &CDecoratorFilter::_q_screenChanged);
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

void CDecoratorFilter::_q_screenChanged(QScreen *screen) {
}

void CDecoratorFilter::_q_deviceRatioChanged(QScreen *screen, double dpi) {
    if (screen == w->screen()) {
        // reloadScreen(qIDec->theme());
    }
}

void CDecoratorFilter::_q_logicalRatioChanged(QScreen *screen, double dpi) {
    if (screen == w->screen()) {
        // reloadScreen(qIDec->theme());
    }
}

static void dfs_extract(const QJsonObject &obj, const QStringList &levels,
                        void(fun)(const QStringList &, const QJsonValue &, void *), void *udata) {
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QStringList nextLevels = QStringList(levels) << it.key();
        const auto &val = it.value();
        if (val.isObject()) {
            dfs_extract(val.toObject(), nextLevels, fun, udata);
        } else {
            fun(nextLevels, val, udata);
        }
    }
};

CDecoratorPrivate::CDecoratorPrivate() {
}

CDecoratorPrivate::~CDecoratorPrivate() {
}

void CDecoratorPrivate::init() {
    Q_Q(CDecorator);

    theme = CDecorator::Dark;
    loc = CDecorator::UnitedStates;

    themeMax = CDecorator::UserTheme;

    // Notify screen add or remove
    q->connect(qApp, &QApplication::screenAdded, q, &CDecorator::_q_screenAdded);
    q->connect(qApp, &QApplication::screenRemoved, q, &CDecorator::_q_screenRemoved);

    // Monitor all screen at first time
    auto screens = qApp->screens();
    for (auto screen : qAsConst(screens)) {
        q->connect(screen, &QScreen::physicalDotsPerInchChanged, q,
                   &CDecorator::_q_deviceRatioChanged);
        q->connect(screen, &QScreen::logicalDotsPerInchChanged, q,
                   &CDecorator::_q_logicalRatioChanged);
    }
}
