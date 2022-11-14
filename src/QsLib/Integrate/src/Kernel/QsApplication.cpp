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

void QsApplication::reloadStrings() {
    emit stringUpdated();
}

void QsApplication::reloadScreen() {
    emit screenUpdated();
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

QString QsApplication::QsFileManagerName() {
#ifdef Q_OS_WINDOWS
    return tr("Explorer");
#elif defined(Q_OS_MAC)
    return tr("Finder");
#else
    return tr("File Manager");
#endif
}

QString QsApplication::allFilesFilter() {
#if defined(Q_OS_WINDOWS)
    return tr("*.*");
#else
    return tr("*");
#endif
}

QsApplication::QsApplication(QsApplicationPrivate &d, int &argc, char **argv)
    : CApplication(d, argc, argv) {
    d.init();
}

void QsApplication::q_screenRatioChanged(double dpi) {
    Q_UNUSED(dpi)
    reloadScreen();
}
