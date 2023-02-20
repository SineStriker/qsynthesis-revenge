#include "QsStartInfo.h"
#include "private/QsStartInfo_p.h"

#include "QsConsole.h"

QsStartInfo::QsStartInfo(QObject *parent) : QsStartInfo(*new QsStartInfoPrivate(), parent) {
}

QsStartInfo::~QsStartInfo() {
}

void QsStartInfo::loadImpl() {
    Q_D(QsStartInfo);
    CStartInfo::loadImpl();
    d->load_helper();
}

QString QsStartInfo::untitledFileName() {
    return tr("Untitled");
}

QString QsStartInfo::unsavedPrefix() {
    return "*";
}

QString QsStartInfo::deletedPrefix() {
    return tr("(Deleted)");
}

QsCoreConsole *QsStartInfo::createConsole(QObject *parent) {
    return new QsConsole(parent);
}

QsStartInfo::QsStartInfo(QsStartInfoPrivate &d, QObject *parent) : CStartInfo(d, parent) {
    d.init();
}
