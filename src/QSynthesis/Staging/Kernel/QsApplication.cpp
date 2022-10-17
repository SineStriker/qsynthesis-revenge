#include "QsApplication.h"
#include "QsApplication_p.h"

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
