#include "LvApplication.h"
#include "private/LvApplication_p.h"

#include "Styles/QCssAnalyzer.h"
#include <QScreen>
#include <QStandardPaths>

LvApplication::LvApplication(int &argc, char **argv)
    : LvApplication(*new LvApplicationPrivate(), argc, argv) {
}

LvApplication::~LvApplication() {
    Q_D(LvApplication);
    d->deinit();
}

void LvApplication::reloadStrings() {
}

void LvApplication::reloadScreen() {
    Q_D(LvApplication);
    setStyleSheet(QCssAnalyzer().apply(d->stylesheets.join("\n"),
                                       primaryScreen()->logicalDotsPerInch() / 96.0 * 0.8));
}

QString LvApplication::mainTitle() const {
    return applicationName();
}

QString LvApplication::windowTitle() const {
    return mainTitle() + QString(" %1").arg(applicationVersion());
}

QString LvApplication::errorTitle() const {
    return tr("Error");
}

QString LvApplication::untitledFileName() const {
    return tr("Untitled");
}

QString LvApplication::unsavedPrefix() const {
    return "*";
}

QString LvApplication::deletedPrefix() const {
    return tr("(Deleted)");
}

QString LvApplication::fileManagerName() const {
#ifdef Q_OS_WINDOWS
    return tr("Explorer");
#elif defined(Q_OS_MAC)
    return tr("Finder");
#else
    return tr("File Manager");
#endif
}

QString LvApplication::allFilesFilter() const {
#if defined(Q_OS_WINDOWS)
    return tr("*.*");
#else
    return tr("*");
#endif
}

QString LvApplication::appDataPath() const {
    Q_D(const LvApplication);
    return d->dataPath;
}

QString LvApplication::appTempPath() const {
    Q_D(const LvApplication);
    return d->tempPath;
}

QString LvApplication::appPluginDir() const {
    Q_D(const LvApplication);
    return d->pluginDir;
}

QString LvApplication::desktopDir() {
    return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
}

LvApplication::LvApplication(LvApplicationPrivate &d, int &argc, char **argv)
    : CApplication(d, argc, argv) {
    d.init();
}

void LvApplication::q_screenRatioChanged(qreal dpi) {
    Q_UNUSED(dpi)
    reloadScreen();
}
