#include "QsStartInfo.h"
#include "private/QsStartInfo_p.h"

QsStartInfo::QsStartInfo(QObject *parent) : QsStartInfo(*new QsStartInfoPrivate(), parent) {
}

QsStartInfo::~QsStartInfo() {
}

void QsStartInfo::load() {
    Q_D(QsStartInfo);
    CStartInfo::load();
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

QsStartInfo::QsStartInfo(QsStartInfoPrivate &d, QObject *parent) : CStartInfo(d, parent) {
    d.init();
}
