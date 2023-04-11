#include "ActionSpec.h"
#include "ActionSpec_p.h"

namespace Core {

    ActionSpecPrivate::ActionSpecPrivate() {
    }

    ActionSpecPrivate::~ActionSpecPrivate() {
    }

    void ActionSpecPrivate::init() {
    }

    ActionSpec::ActionSpec(const QString &id, QObject *parent) : ActionSpec(*new ActionSpecPrivate(), id, parent) {
    }

    ActionSpec::~ActionSpec() {
    }

    QString ActionSpec::id() const {
        Q_D(const ActionSpec);
        return d->id;
    }

    QString ActionSpec::displayName() const {
        Q_D(const ActionSpec);
        return d->displayName;
    }

    void ActionSpec::setDisplayName(const QString &displayName) {
        Q_D(ActionSpec);
        d->displayName = displayName;
        emit displayNameChanged(displayName);
    }

    QString ActionSpec::description() const {
        Q_D(const ActionSpec);
        return d->description;
    }

    void ActionSpec::setDescription(const QString &description) {
        Q_D(ActionSpec);
        d->description = description;
        emit descriptionChanged(description);
    }

    QList<QKeySequence> ActionSpec::shortcuts() const {
        Q_D(const ActionSpec);
        return d->shortcuts;
    }

    void ActionSpec::setShortcuts(const QList<QKeySequence> &shortcuts) {
        Q_D(ActionSpec);
        d->shortcuts = shortcuts;
        emit shortcutsChanged(shortcuts);
    }

    ActionSpec::ActionSpec(ActionSpecPrivate &d, const QString &id, QObject *parent) : QObject(parent), d_ptr(&d) {
        d.q_ptr = this;
        d.id = id;

        d.init();
    }

}
