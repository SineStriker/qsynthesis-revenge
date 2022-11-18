#include "LvElemApplication.h"
#include "private/LvElemApplication_p.h"

#include "Styles/QCssAnalyzer.h"

#include <QScreen>
#include <QStandardPaths>

LvElemApplication::LvElemApplication(int &argc, char **argv)
    : LvElemApplication(*new LvElemApplicationPrivate(), argc, argv) {
}

LvElemApplication::~LvElemApplication() {
    Q_D(LvElemApplication);
    d->deinit();
}

void LvElemApplication::reloadStrings(int locale) {
    Q_D(LvElemApplication);

    switch (locale) {
        case MultimodeHandle::UnitedStates:
            d->ll->eliminate();
            break;
        case MultimodeHandle::China:
            d->ll->translateOnly(":/translations/LvElem_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->translateOnly(":/translations/LvElem_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->translateOnly(":/translations/LvElem_ja_JP.qm");
            break;
        default:
            break;
    }
}

void LvElemApplication::reloadScreen(int theme) {
    Q_D(LvElemApplication);
    Q_UNUSED(theme);

    d->ld->addTheme(":/themes/light/base-light.qss");

    setStyleSheet(QCssAnalyzer().apply(d->ld->stylesheets(),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

LvElemApplication::LvElemApplication(LvElemApplicationPrivate &d, int &argc, char **argv)
    : QsApplication(d, argc, argv) {
    d.init();
}
