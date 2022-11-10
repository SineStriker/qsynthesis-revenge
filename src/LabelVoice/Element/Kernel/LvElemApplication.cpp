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

void LvElemApplication::reloadStrings() {
}

void LvElemApplication::reloadScreen() {
    Q_D(LvElemApplication);
    setStyleSheet(QCssAnalyzer().apply(d->stylesheets.join("\n"),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

QString LvElemApplication::mainTitle() {
    return applicationName();
}

QString LvElemApplication::windowTitle() {
    return mainTitle() + QString(" %1").arg(applicationVersion());
}

QString LvElemApplication::errorTitle() {
    return tr("Error");
}

QString LvElemApplication::untitledFileName() {
    return tr("Untitled");
}

QString LvElemApplication::unsavedPrefix() {
    return "*";
}

QString LvElemApplication::deletedPrefix() {
    return tr("(Deleted)");
}

QString LvElemApplication::fileManagerName() {
#ifdef Q_OS_WINDOWS
    return tr("Explorer");
#elif defined(Q_OS_MAC)
    return tr("Finder");
#else
    return tr("File Manager");
#endif
}

QString LvElemApplication::allFilesFilter() {
#if defined(Q_OS_WINDOWS)
    return tr("*.*");
#else
    return tr("*");
#endif
}

LvElemApplication::LvElemApplication(LvElemApplicationPrivate &d, int &argc, char **argv)
    : CApplication(d, argc, argv) {
    d.init();
}

void LvElemApplication::q_screenRatioChanged(qreal dpi) {
    Q_UNUSED(dpi)
    reloadScreen();
}
