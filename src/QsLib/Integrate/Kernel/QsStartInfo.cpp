#include "QsStartInfo.h"
#include "private/QsStartInfo_p.h"

QsStartInfo::QsStartInfo(QObject *parent) : QsStartInfo(*new QsStartInfoPrivate(), parent) {
}

QsStartInfo::~QsStartInfo() {
}

void QsStartInfo::parse() {
    Q_D(QsStartInfo);
    CStartInfo::parse();
    d->parse_helper();
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