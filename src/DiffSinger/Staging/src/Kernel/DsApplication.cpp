#include "DsApplication.h"

#include "private/DsApplication_p.h"

#include "Styles/QCssAnalyzer.h"
#include "ViewHelper.h"

#include "Kernel/DsStartupInfo.h"

#include <QScreen>

DsApplication::DsApplication(int &argc, char **argv)
    : DsApplication(*new DsApplicationPrivate(), argc, argv) {
}

DsApplication::~DsApplication() {
    Q_D(DsApplication);
    d->deinit();
}

void DsApplication::reloadStrings(int locale) {
    Q_D(DsApplication);

    switch (locale) {
        case MultimodeHandle::UnitedStates:
            d->ll->eliminate();
            break;
        case MultimodeHandle::China:
            d->ll->eliminate();
            d->ll->translate(qAppConf->appDir(DsDistConfig::Locales) + "/qt_zh_CN.qm");
            d->ll->translate(qAppConf->appDir(DsDistConfig::Locales) + "/qtbase_zh_CN.qm");
            d->ll->translate(":/translations/DsStaging_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->eliminate();
            d->ll->translate(qAppConf->appDir(DsDistConfig::Locales) + "/qt_zh_TW.qm");
            d->ll->translate(qAppConf->appDir(DsDistConfig::Locales) + "/qtbase_zh_TW.qm");
            d->ll->translate(":/translations/DsStaging_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->eliminate();
            d->ll->translate(qAppConf->appDir(DsDistConfig::Locales) + "/qt_zh_ja.qm");
            d->ll->translate(qAppConf->appDir(DsDistConfig::Locales) + "/qtbase_ja.qm");
            d->ll->translate(":/translations/DsStaging_ja_JP.qm");
            break;
        default:
            break;
    }
}

void DsApplication::reloadScreen(int theme) {
    Q_UNUSED(theme);

    Q_D(DsApplication);
    d->ld->addTheme(":/themes/light/home-light.qss");
    d->ld->addTheme(":/themes/light/piano-light.qss");

    setStyleSheet(styleSheet() + "\n\n" +
                  QCssAnalyzer().apply(d->ld->stylesheets(),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

void DsApplication::receiveMessage(quint32 instanceId, const QByteArray &message) {
    Q_UNUSED(instanceId);
    Q_UNUSED(message);

    Q_D(DsApplication);
    // View::bringWindowToForeground(d->windowMgr->firstWindow());
}

DsApplication::DsApplication(DsApplicationPrivate &d, int &argc, char **argv)
    : DsElemApplication(d, argc, argv) {
    d.init();
}
