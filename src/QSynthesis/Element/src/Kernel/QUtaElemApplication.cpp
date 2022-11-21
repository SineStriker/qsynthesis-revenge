#include "QUtaElemApplication.h"
#include "private/QUtaElemApplication_p.h"

#include "Styles/QCssAnalyzer.h"

#include <QScreen>
#include <QStandardPaths>

QUtaElemApplication::QUtaElemApplication(int &argc, char **argv)
    : QUtaElemApplication(*new QUtaElemApplicationPrivate(), argc, argv) {
}

QUtaElemApplication::~QUtaElemApplication() {
    Q_D(QUtaElemApplication);
    d->deinit();
}

void QUtaElemApplication::reloadStrings(int locale) {
    Q_D(QUtaElemApplication);

    switch (locale) {
        case MultimodeHandle::UnitedStates:
            d->ll->eliminate();
            break;
        case MultimodeHandle::China:
            d->ll->translateOnly(":/translations/QUtaElem_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->translateOnly(":/translations/QUtaElem_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->translateOnly(":/translations/QUtaElem_ja_JP.qm");
            break;
        default:
            break;
    }
}

void QUtaElemApplication::reloadScreen(int theme) {
    Q_D(QUtaElemApplication);
    Q_UNUSED(theme);

    d->ld->addTheme(":/themes/light/base-light.qss");

    setStyleSheet(QCssAnalyzer().apply(d->ld->stylesheets(),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

QUtaElemApplication::QUtaElemApplication(QUtaElemApplicationPrivate &d, int &argc, char **argv)
    : QsApplication(d, argc, argv) {
    d.init();
}
