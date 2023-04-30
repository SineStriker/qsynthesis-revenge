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

    QString DocumentSpec::displayName() const {
        Q_D(const DocumentSpec);
        return d->displayName;
    }

    void DocumentSpec::setDisplayName(const QString &displayName) {
        Q_D(DocumentSpec);
        d->displayName = displayName;
    }

    QString DocumentSpec::saveFilter() const {
        return filter();
    }

    bool DocumentSpec::canRecover() const {
        return false;
    }

    bool DocumentSpec::recover(const QString &logDir, const QString &fileName) {
        return false;
    }

    DocumentSpec::DocumentSpec(DocumentSpecPrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

}
