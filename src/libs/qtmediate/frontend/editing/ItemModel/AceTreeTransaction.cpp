#include "AceTreeTransaction.h"

#include <QDebug>
#include <QPointer>
#include <QStack>

#define myWarning(func) (qWarning().nospace() << "AceTreeTransaction::" << (func) << "(): ").maybeSpace()

class AceTreeTransactionPrivate {
public:
    AceTreeTransactionPrivate(AceTreeModel *model, AceTreeTransaction *q) : q(q), model(model) {
        state = AceTreeTransaction::Idle;
        current = -1;
        undoIndex = 0;

        if (model->property("ace_tree_transaction").toBool()) {
            myWarning("AceTreeTransaction") << "A model cannot be managed by two transaction managers simultaneously";
            this->model.clear();
            return;
        }

        model->setProperty("ace_tree_transaction", true);
    }

    ~AceTreeTransactionPrivate() {
        if (model) {
            model->setProperty("ace_tree_transaction", QVariant());
        }
    }

    AceTreeTransaction *q;
    QPointer<AceTreeModel> model;

    AceTreeTransaction::State state;

    struct Operation {
        QString key;
        QMap<QString, QString> args;

        int begin;
        int count;

        Operation() : begin(-1), count(0){};
        ~Operation() = default;

        bool isValid() const {
            return begin >= 0;
        }
    };

    int current;

    QStack<Operation> undoStack;
    int undoIndex;
};

AceTreeTransaction::AceTreeTransaction(AceTreeModel *model, QObject *parent)
    : QObject(parent), d(new AceTreeTransactionPrivate(model, this)) {
}

AceTreeTransaction::~AceTreeTransaction() {
    delete d;
}

bool AceTreeTransaction::isValid() const {
    return d->model != nullptr;
}

using DescriptionGetterFunc = QHash<QString, AceTreeTransaction::FormatDescription>;
Q_GLOBAL_STATIC(DescriptionGetterFunc, descGetterIndexes)

bool AceTreeTransaction::hasDescriptionFormatter(const QString &key) {
    return descGetterIndexes->contains(key);
}

void AceTreeTransaction::registerDescriptionFormatter(const QString &key, FormatDescription func) {
    if (!func) {
        descGetterIndexes->remove(key);
        return;
    }
    descGetterIndexes->insert(key, func);
}

AceTreeTransaction::State AceTreeTransaction::state() const {
    return d->state;
}

void AceTreeTransaction::beginTransaction() {
    if (!isIdle()) {
        myWarning(__func__) << "Cannot start transaction, current state:" << state();
        return;
    }

    d->state = Transaction;

    // Save current step
    d->current = d->model->currentStep();
}

void AceTreeTransaction::abortTransaction() {
    if (!inTransaction()) {
        myWarning(__func__) << "No transaction in execution";
        return;
    }

    // Abort
    d->model->setCurrentStep(d->current);
    d->model->truncateForwardSteps();
    d->current = -1;

    d->state = Idle;
}

void AceTreeTransaction::endTransaction(const QString &key, const QMap<QString, QString> &args) {
    if (!inTransaction()) {
        myWarning(__func__) << "No transaction in execution";
        return;
    }

    AceTreeTransactionPrivate::Operation op;
    op.begin = d->current;
    op.count = d->model->currentStep() - op.begin;
    op.key = key;
    op.args = args;

    d->current = -1;

    // Commit
    d->undoStack.push(op);
    d->undoIndex++;

    d->state = Idle;

    emit indexChanged(index());
}

int AceTreeTransaction::count() const {
    return d->undoStack.size();
}

int AceTreeTransaction::index() const {
    return d->undoIndex;
}

QString AceTreeTransaction::description(int index) const {
    if (index >= 0 && index <= d->undoStack.size())
        return {};

    const auto &op = d->undoStack.at(index);
    auto func = descGetterIndexes->value(op.key);
    return func ? func(op.args) : QString();
}

void AceTreeTransaction::undo() {
    if (!isIdle()) {
        myWarning(__func__) << "Cannot executing undo, current state:" << state();
        return;
    }
    if (!canUndo()) {
        myWarning(__func__) << "No operation to undo";
        return;
    }

    d->state = Undo;

    const auto &op = d->undoStack.at(d->undoIndex - 1);
    d->undoIndex--;
    d->model->setCurrentStep(op.begin);

    d->state = Idle;

    emit indexChanged(index());
}

void AceTreeTransaction::redo() {
    if (!isIdle()) {
        myWarning(__func__) << "Cannot executing redo, current state:" << state();
        return;
    }
    if (!canRedo()) {
        myWarning(__func__) << "No operation to undo";
        return;
    }

    d->state = Redo;

    const auto &op = d->undoStack.at(d->undoIndex);
    d->undoIndex++;
    d->model->setCurrentStep(op.begin);

    d->state = Idle;

    emit indexChanged(index());
}
