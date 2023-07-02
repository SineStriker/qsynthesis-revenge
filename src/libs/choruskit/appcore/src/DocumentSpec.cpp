#include "DocumentSpec.h"
#include "DocumentSpec_p.h"

#include <QDebug>

#include "ICoreBase.h"

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

    void DocumentSpec::setDisplayName(const QMDisplayString &displayName) {
        Q_D(DocumentSpec);
        d->displayName = displayName;
    }

    QString DocumentSpec::description() const {
        Q_D(const DocumentSpec);
        return d->description;
    }

    void DocumentSpec::setDescription(const QMDisplayString &description) {
        Q_D(DocumentSpec);
        d->description = description;
    }

    QIcon DocumentSpec::icon() const {
        Q_D(const DocumentSpec);
        return d->icon;
    }

    void DocumentSpec::setIcon(const QIcon &icon) {
        Q_D(DocumentSpec);
        d->icon = icon;
    }

    QString DocumentSpec::saveFilter() const {
        return filter();
    }

    bool DocumentSpec::open(const QString &fileName, QWidget *parent) {
        auto doc = ICoreBase::instance()->documentSystem()->searchDocument(fileName);
        if (doc) {
            doc->raise();
            return true;
        }
        return false;
    }

    DocumentSpec::DocumentSpec(DocumentSpecPrivate &d, const QString &id, QObject *parent)
        : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

}
