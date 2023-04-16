#include "DocumentSpec.h"
#include "DocumentSpec_p.h"

namespace Core {

    DocumentSpecPrivate::DocumentSpecPrivate() {
    }

    DocumentSpecPrivate::~DocumentSpecPrivate() {
    }

    void DocumentSpecPrivate::init() {
    }

    DocumentSpec::DocumentSpec(const QString &id, QObject *parent)
        : DocumentSpec(*new DocumentSpecPrivate(), id, parent) {
    }

    DocumentSpec::~DocumentSpec() {
    }

    QString DocumentSpec::id() const {
        Q_D(const DocumentSpec);
        return d->id;
    }

    QString DocumentSpec::saveFilter() const {
        return filter();
    }

    DocumentSpec::DocumentSpec(DocumentSpecPrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

}
