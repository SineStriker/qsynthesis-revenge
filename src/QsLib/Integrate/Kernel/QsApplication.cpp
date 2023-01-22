#include "QsApplication.h"
#include "private/QsApplication_p.h"

#include "QsSystem.h"

#include <QScreen>
#include <QStandardPaths>
#include <QWidget>

QsApplication::QsApplication(int &argc, char **argv)
    : QsApplication(*new QsApplicationPrivate(), argc, argv) {
}

QsApplication::~QsApplication() {
    Q_D(QsApplication);
    d->deinit();
}

QString QsApplication::untitledFileName() {
    return tr("Untitled");
}

QString QsApplication::unsavedPrefix() {
    return "*";
}

QString QsApplication::deletedPrefix() {
    return tr("(Deleted)");
}

QsApplication::QsApplication(QsApplicationPrivate &d, int &argc, char **argv)
    : CApplication(argc, argv), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
