#include "CDecorator.h"
#include "private/CDecorator_p.h"

Q_SINGLETON_DECLARE(CDecorator)

CDecorator::CDecorator(QObject *parent) : CDecorator(*new CDecoratorPrivate(), parent) {
}

CDecorator::~CDecorator() {
}

int CDecorator::theme() const {
    Q_D(const CDecorator);
    return d->theme;
}

void CDecorator::setTheme(int theme) {
    Q_D(CDecorator);

    int org = d->theme;
    d->theme = static_cast<Theme>(theme);
    d->loadTheme();
    emit themeChanged(org);
}

void CDecorator::addTheme(const QString &key, const QStringList &qss,
                          const QMap<int, QStringList> &conf) {
}

void CDecorator::removeTheme(const QString &key) {
}

int CDecorator::registerNewTheme() {
    Q_D(CDecorator);
    return ++d->themeMax;
}

int CDecorator::locale() const {
    Q_D(const CDecorator);
    return d->loc;
}

void CDecorator::setLocale(int locale) {
}

void CDecorator::addLocale(const QString &key, const QMap<int, QStringList> &paths) {
}

void CDecorator::removeLocale(const QString &key) {
}

CDecorator::CDecorator(CDecoratorPrivate &d, QObject *parent) : QObject(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}

void CDecorator::_q_screenAdded(QScreen *screen) {
    connect(screen, &QScreen::physicalDotsPerInchChanged, this, &CDecorator::_q_deviceRatioChanged);
    connect(screen, &QScreen::logicalDotsPerInchChanged, this, &CDecorator::_q_logicalRatioChanged);
}

void CDecorator::_q_screenRemoved(QScreen *screen) {
    disconnect(screen, &QScreen::physicalDotsPerInchChanged, this,
               &CDecorator::_q_deviceRatioChanged);
    disconnect(screen, &QScreen::logicalDotsPerInchChanged, this,
               &CDecorator::_q_logicalRatioChanged);
}

void CDecorator::_q_deviceRatioChanged(double dpi) {
    emit deviceRatioChanged(qobject_cast<QScreen *>(sender()), dpi);
}

void CDecorator::_q_logicalRatioChanged(double dpi) {
    emit logicalRatioChanged(qobject_cast<QScreen *>(sender()), dpi);
}
