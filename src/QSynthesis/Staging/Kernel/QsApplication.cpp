#include "QsApplication.h"
#include "private/QsApplication_p.h"

#ifdef Q_QDOC
QsApplication::QsApplication(int m_argc, char **argv)
    : QsApplication(*new QsApplicationPrivate(), argc, argv) {
#else
QsApplication::QsApplication(int &argc, char **argv, int _internal)
    : QsApplication(*new QsApplicationPrivate(), argc, argv, _internal){
#endif
}

QsApplication::~QsApplication() {
    Q_D(QsApplication);
    d->deinit();
}

void QsApplication::reloadStrings() {
}

void QsApplication::reloadScreen() {
}

QString QsApplication::mainTitle() const {
    return applicationName();
}

QString QsApplication::windowTitle() const {
    return mainTitle() + QString(" %1").arg(applicationVersion());
}

QString QsApplication::errorTitle() const {
    return tr("Error");
}

QString QsApplication::untitledFileName() const {
    return tr("Untitled");
}

QString QsApplication::unsavedPrefix() const {
    return "*";
}

QString QsApplication::deletedPrefix() const {
    return tr("(Deleted)");
}

QString QsApplication::fileManagerName() const {
#ifdef Q_OS_WINDOWS
    return tr("Explorer");
#elif defined(Q_OS_MAC)
    return tr("Finder");
#else
    return tr("File Manager");
#endif
}

QString QsApplication::appDataPath() const {
    Q_D(const QsApplication);
    return d->dataPath;
}

QString QsApplication::appTempPath() const {
    Q_D(const QsApplication);
    return d->tempPath;
}

#ifdef Q_QDOC
QsApplication::QsApplication(QsApplicationPrivate &d, int &argc, char **argv)
    : CApplication(d, argc, argv) {
#else
QsApplication::QsApplication(QsApplicationPrivate &d, int &argc, char **argv, int _internal)
    : CApplication(d, argc, argv, _internal) {
#endif
    d.init();
}

void QsApplication::q_screenRatioChanged(qreal dpi) {
    Q_UNUSED(dpi)
    reloadScreen();
}
