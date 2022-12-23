#include "DsElemApplication.h"
#include "private/DsElemApplication_p.h"

#include "Styles/QCssAnalyzer.h"

#include <QScreen>
#include <QStandardPaths>

DsElemApplication::DsElemApplication(int &argc, char **argv)
    : DsElemApplication(*new DsElemApplicationPrivate(), argc, argv) {
}

DsElemApplication::~DsElemApplication() {
    Q_D(DsElemApplication);
    d->deinit();
}

void DsElemApplication::reloadStrings(int locale) {
    Q_D(DsElemApplication);

    switch (locale) {
        case MultimodeHandle::UnitedStates:
            d->ll->eliminate();
            break;
        case MultimodeHandle::China:
            d->ll->translateOnly(":/translations/DsElem_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->translateOnly(":/translations/DsElem_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->translateOnly(":/translations/DsElem_ja_JP.qm");
            break;
        default:
            break;
    }
}

void DsElemApplication::reloadScreen(int theme) {
    Q_D(DsElemApplication);
    Q_UNUSED(theme);

    d->ld->addTheme(":/themes/light/base-light.qss");

    setStyleSheet(QCssAnalyzer().apply(d->ld->stylesheets(),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

DsElemApplication::DsElemApplication(DsElemApplicationPrivate &d, int &argc, char **argv)
    : QsApplication(d, argc, argv) {
    d.init();
}
