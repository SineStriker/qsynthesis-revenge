#include "QUtaApplication.h"

#include "private/QUtaApplication_p.h"

#include "Styles/QCssAnalyzer.h"
#include "ViewHelper.h"

#include "Kernel/QUtaStartupInfo.h"

#include <QScreen>

QUtaApplication::QUtaApplication(int &argc, char **argv)
    : QUtaApplication(*new QUtaApplicationPrivate(), argc, argv) {
}

QUtaApplication::~QUtaApplication() {
    Q_D(QUtaApplication);
    d->deinit();
}

void QUtaApplication::reloadStrings(int locale) {
    Q_D(QUtaApplication);

    switch (locale) {
        case MultimodeHandle::UnitedStates:
            d->ll->eliminate();
            break;
        case MultimodeHandle::China:
            d->ll->eliminate();
            d->ll->translate(qAppConf->appDir(QUtaDistConfig::Locales) + "/qt_zh_CN.qm");
            d->ll->translate(qAppConf->appDir(QUtaDistConfig::Locales) + "/qtbase_zh_CN.qm");
            d->ll->translate(":/translations/LvStaging_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->eliminate();
            d->ll->translate(qAppConf->appDir(QUtaDistConfig::Locales) + "/qt_zh_TW.qm");
            d->ll->translate(qAppConf->appDir(QUtaDistConfig::Locales) + "/qtbase_zh_TW.qm");
            d->ll->translate(":/translations/LvStaging_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->eliminate();
            d->ll->translate(qAppConf->appDir(QUtaDistConfig::Locales) + "/qt_zh_ja.qm");
            d->ll->translate(qAppConf->appDir(QUtaDistConfig::Locales) + "/qtbase_ja.qm");
            d->ll->translate(":/translations/LvStaging_ja_JP.qm");
            break;
        default:
            break;
    }
}

void QUtaApplication::reloadScreen(int theme) {
    Q_UNUSED(theme);

    Q_D(QUtaApplication);
    d->ld->addTheme(":/themes/light/home-light.qss");
    d->ld->addTheme(":/themes/light/piano-light.qss");

    setStyleSheet(styleSheet() + "\n\n" +
                  QCssAnalyzer().apply(d->ld->stylesheets(),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

void QUtaApplication::receiveMessage(quint32 instanceId, const QByteArray &message) {
    Q_UNUSED(instanceId);
    Q_UNUSED(message);

    Q_D(QUtaApplication);
    // View::bringWindowToForeground(d->windowMgr->firstWindow());
}

QUtaApplication::QUtaApplication(QUtaApplicationPrivate &d, int &argc, char **argv)
    : QUtaElemApplication(d, argc, argv) {
    d.init();
}
