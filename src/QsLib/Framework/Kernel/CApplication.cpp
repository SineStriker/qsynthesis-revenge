#include "CApplication.h"

#include "CStartupInfo.h"
#include "private/CApplication_p.h"

#include <QDateTime>
#include <QMouseEvent>
#include <QWidget>

extern Q_DECL_IMPORT QWidget *qt_button_down;

CApplication::CApplication(int &argc, char **argv)
    : CApplication(*new CApplicationPrivate(), argc, argv) {
}

CApplication::~CApplication() {
    Q_D(CApplication);
    d->deinit();
}
QWidget *CApplication::implicitMouseGrabber() const {
    return qt_button_down;
}

QString CApplication::mainTitle() {
    return qIStup->mainTitle();
}

QString CApplication::windowTitle() {
    return qIStup->windowTitle();
}

QString CApplication::errorTitle() {
    return qIStup->errorTitle();
}

QString CApplication::sysFileManagerName() {
#ifdef Q_OS_WINDOWS
    return tr("Explorer");
#elif defined(Q_OS_MAC)
    return tr("Finder");
#else
    return tr("File Manager");
#endif
}

QString CApplication::sysRootUserName() {
#if defined(Q_OS_WINDOWS)
    return tr("Administrator");
#else
    return tr("Root User");
#endif
}

QString CApplication::allFilesFilter() {
#if defined(Q_OS_WINDOWS)
    return "*.*";
#else
    return "*";
#endif
}

CApplication::CApplication(CApplicationPrivate &d, int &argc, char **argv)
    : QApplication(argc, argv), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}

bool CApplication::notify(QObject *obj, QEvent *event) {
    return QApplication::notify(obj, event);
}

void CApplication::newStartedInstance() {
}

void CApplication::receiveMessage(quint32 instanceId, const QByteArray &message) {
    Q_UNUSED(instanceId);
    Q_UNUSED(message);
}

void CApplication::_q_instanceStarted() {
    newStartedInstance();
}

void CApplication::_q_messageReceived(quint32 instanceId, QByteArray message) {
    receiveMessage(instanceId, message);
}
