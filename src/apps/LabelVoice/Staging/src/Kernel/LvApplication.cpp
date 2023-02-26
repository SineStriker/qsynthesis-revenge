#include "LvApplication.h"

#include "private/LvApplication_p.h"

#include "Styles/QCssAnalyzer.h"
#include "ViewHelper.h"
#include "QMSystem.h"

#include "Kernel/LvStartupInfo.h"

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
            d->ll->eliminate();
            d->ll->translate(qsConf->appDir(LvDistConfig::Locales) + "/qt_zh_CN.qm");
            d->ll->translate(qsConf->appDir(LvDistConfig::Locales) + "/qtbase_zh_CN.qm");
            d->ll->translate(":/translations/LvStaging_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->eliminate();
            d->ll->translate(qsConf->appDir(LvDistConfig::Locales) + "/qt_zh_TW.qm");
            d->ll->translate(qsConf->appDir(LvDistConfig::Locales) + "/qtbase_zh_TW.qm");
            d->ll->translate(":/translations/LvStaging_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->eliminate();
            d->ll->translate(qsConf->appDir(LvDistConfig::Locales) + "/qt_zh_ja.qm");
            d->ll->translate(qsConf->appDir(LvDistConfig::Locales) + "/qtbase_ja.qm");
            d->ll->translate(":/translations/LvStaging_ja_JP.qm");
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
                                       primaryScreen()->logicalDotsPerInch() / Sys::osUnitDpi() * 0.8));
}

void LvApplication::receiveMessage(quint32 instanceId, const QByteArray &message) {
    Q_UNUSED(instanceId);
    Q_UNUSED(message);

    Q_D(LvApplication);
    View::bringWindowToForeground(d->windowMgr->firstWindow());
}

LvApplication::LvApplication(LvApplicationPrivate &d, int &argc, char **argv)
    : LvElemApplication(d, argc, argv) {
    d.init();
}
