#include "QsApplication.h"
#include "private/QsApplication_p.h"

#include <QScreen>
#include <QStandardPaths>

QsApplication::QsApplication(int &argc, char **argv)
    : QsApplication(*new QsApplicationPrivate(), argc, argv) {
}

QsApplication::~QsApplication() {
    Q_D(QsApplication);
    d->deinit();
}

void QsApplication::reloadStrings(int locale) {
    Q_D(QsApplication);

    switch (locale) {
        case MultimodeHandle::UnitedStates:
            d->ll->eliminate();
            break;
        case MultimodeHandle::China:
            d->ll->translateOnly(":/translations/QsIntegrate_zh_CN.qm");
            break;
        case MultimodeHandle::HongKong:
            d->ll->translateOnly(":/translations/QsIntegrate_zh_HK.qm");
            break;
        case MultimodeHandle::Japan:
            d->ll->translateOnly(":/translations/QsIntegrate_ja_JP.qm");
            break;
        default:
            break;
    }
}

void QsApplication::reloadScreen(int theme) {
    Q_UNUSED(theme);
}

QString QsApplication::mainTitle() {
    return applicationName();
}

QString QsApplication::windowTitle() {
    return mainTitle() + QString(" %1").arg(applicationVersion());
}

QString QsApplication::errorTitle() {
    return tr("Error");
}

QString QsApplication::untitledFileName() {
    return tr("Untitled");
}

QString QsApplication::unsavedPrefix() {
    return "*";
}

QString QsApplication::deletedPrefix() {
    return tr("(Deleted)");
}

QString QsApplication::sysFileManagerName() {
#ifdef Q_OS_WINDOWS
    return tr("Explorer");
#elif defined(Q_OS_MAC)
    return tr("Finder");
#else
    return tr("File Manager");
#endif
}

QString QsApplication::sysRootUserName() {
#if defined(Q_OS_WINDOWS)
    return tr("Administrator");
#else
    return tr("Root User");
#endif
}

QString QsApplication::allFilesFilter() {
#if defined(Q_OS_WINDOWS)
    return "*.*";
#else
    return "*";
#endif
}

QsApplication::QsApplication(QsApplicationPrivate &d, int &argc, char **argv)
    : CApplication(d, argc, argv) {
    d.init();

    Q_TR_NOTIFY(QsApplication);
    Q_SS_NOTIFY(QsApplication);

    d.init2();
}

void QsApplication::q_screenRatioChanged(double dpi) {
    Q_UNUSED(dpi)
    qMMH->setTheme(qMMH->theme());
}
