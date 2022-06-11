#include "QsApplication.h"
#include "QsApplication_p.h"

#ifdef Q_QDOC
QsApplication::QsApplication(int m_argc, char **argv)
    : QsApplication(*new QsApplicationPrivate(), argc, argv) {
}
#else
QsApplication::QsApplication(int &argc, char **argv, int _internal)
    : QsApplication(*new QsApplicationPrivate(), argc, argv, _internal) {
}
#endif

QsApplication::~QsApplication() {
    Q_D(QsApplication);
    d->deinit();
}

#ifdef Q_QDOC
QsApplication::QsApplication(QsApplicationPrivate &d, int &argc, char **argv)
    : CApplication(d, argc, argv) {
    d.init();
}
#else
QsApplication::QsApplication(QsApplicationPrivate &d, int &argc, char **argv, int _internal)
    : CApplication(d, argc, argv, _internal) {
    d.init();
}
#endif

void QsApplication::q_screenRatioChanged(qreal dpi) {
    Q_UNUSED(dpi)
    qTheme->loadTheme(qRecordCData.themeIndex);
}
