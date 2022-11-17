#include "LvApplication.h"

#include "private/LvApplication_p.h"

#include "Styles/QCssAnalyzer.h"

#include <QScreen>

LvApplication::LvApplication(int &argc, char **argv)
    : LvApplication(*new LvApplicationPrivate(), argc, argv) {
}

LvApplication::~LvApplication() {
    Q_D(LvApplication);
    d->deinit();
}

void LvApplication::reloadStrings(int locale) {
    Q_D(LvApplication);

    switch (locale) {
        case MultimodeHandle::UnitedStates:
            d->ll->eliminate();
            break;
        case MultimodeHandle::China:
            d->ll->translateOnly(":/translations/LvStaging_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->translateOnly(":/translations/LvStaging_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->translateOnly(":/translations/LvStaging_ja_JP.qm");
            break;
        default:
            break;
    }
}

void LvApplication::reloadScreen(int theme) {
    Q_UNUSED(theme);

    Q_D(LvApplication);
    d->ld->addTheme(":/themes/light/home-light.qss");
    d->ld->addTheme(":/themes/light/piano-light.qss");

    setStyleSheet(styleSheet() + "\n\n" +
                  QCssAnalyzer().apply(d->ld->stylesheets(),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

QStringList LvApplication::filenameArgs() const {
    Q_D(const LvApplication);
    return d->filenames;
}

LvApplication::LvApplication(LvApplicationPrivate &d, int &argc, char **argv)
    : LvElemApplication(d, argc, argv) {
    d.init();

    Q_TR_NOTIFY(LvApplication);
    Q_SS_NOTIFY(LvApplication);

    d.init2();
}
