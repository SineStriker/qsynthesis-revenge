#include "QsCoreStartInfo.h"
#include "private/QsCoreStartInfo_p.h"

#include <QCoreApplication>
#include <QMetaType>

#include "QsCoreConfig.h"
#include "QsCoreConsole.h"
#include "QsCoreDecorator.h"

Q_SINGLETON_DECLARE(QsCoreStartInfo)

QsCoreStartInfo::QsCoreStartInfo(QObject *parent) : QsCoreStartInfo(*new QsCoreStartInfoPrivate(), parent) {
}

QsCoreStartInfo::~QsCoreStartInfo() {
}

void QsCoreStartInfo::loadImpl() {
    Q_D(QsCoreStartInfo);
    d->load_helper();
}

void QsCoreStartInfo::saveImpl() {
    Q_D(QsCoreStartInfo);
    d->save_helper();
}

bool QsCoreStartInfo::isAboutToQuit() const {
    Q_D(const QsCoreStartInfo);
    return d->isAboutToQuit;
}

QString QsCoreStartInfo::mainTitle() const {
    return qApp->applicationName();
}

QString QsCoreStartInfo::windowTitle() const {
    return mainTitle() + QString(" %1").arg(qApp->applicationVersion());
}

QString QsCoreStartInfo::errorTitle() const {
    return QCoreApplication::tr("Error");
}

QsCoreConsole *QsCoreStartInfo::createConsole(QObject *parent) {
    return new QsCoreConsole(parent);
}

QsCoreDecorator *QsCoreStartInfo::createDecorator(QObject *parent) {
    return new QsCoreDecorator(parent);
}

QsCoreConfig *QsCoreStartInfo::creatDistConfig() {
    return new QsCoreConfig();
}

QsCoreStartInfo::QsCoreStartInfo(QsCoreStartInfoPrivate &d, QObject *parent) : QMDisposable(parent), d_ptr(&d) {
    construct();

    d.q_ptr = this;
    d.init();

    allowRootUser = false;
    allowSecondary = false;

    parser.addHelpOption();
    parser.addVersionOption();
}

void QsCoreStartInfo::newStartedInstance() {
}

void QsCoreStartInfo::receiveMessage(quint32 instanceId, const QByteArray &message) {
}

void QsCoreStartInfo::clean() {
}

void QsCoreStartInfo::_q_instanceStarted() {
    newStartedInstance();
}

void QsCoreStartInfo::_q_messageReceived(quint32 instanceId, QByteArray message) {
    receiveMessage(instanceId, message);
}

void QsCoreStartInfo::_q_aboutToQuit() {
    Q_D(QsCoreStartInfo);
    d->isAboutToQuit = true;
    clean();
}
