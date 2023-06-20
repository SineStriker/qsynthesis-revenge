#ifndef CHORUSKIT_ACETREETRANSACTION_H
#define CHORUSKIT_ACETREETRANSACTION_H

#include <QMDisplayString.h>

#include "AceTreeModel.h"

class AceTreeTransactionPrivate;

class QMEDITING_EXPORT AceTreeTransaction : public AceTreeRecoverable {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeTransaction)
public:
    explicit AceTreeTransaction(AceTreeModel *model, QObject *parent = nullptr);
    ~AceTreeTransaction();

    bool isValid() const;

public:
    enum State {
        Idle,
        Transaction,
        Undo,
        Redo,
    };
    Q_ENUM(State)

    State state() const;

    inline bool isIdle() const;
    inline bool inTransaction() const;
    inline bool undoExecuting() const;
    inline bool redoExecuting() const;

    void beginTransaction();
    void abortTransaction();
    void endTransaction(const QString &name, const QMap<QString, QString> &attributes = {});

    int count() const;
    int index() const;

    QString name(int index) const;
    QString attribute(int index, const QString &key) const;

    void undo();
    void redo();

    inline bool canUndo() const;
    inline bool canRedo() const;

    bool recover(const QByteArray &data) override;

signals:
    void indexChanged(int index);

protected:
    AceTreeTransaction(AceTreeTransactionPrivate &d, AceTreeModel *model, QObject *parent = nullptr);
};

bool AceTreeTransaction::isIdle() const {
    return state() == Idle;
}

bool AceTreeTransaction::inTransaction() const {
    return state() == Transaction;
}

bool AceTreeTransaction::undoExecuting() const {
    return state() == Undo;
}

bool AceTreeTransaction::redoExecuting() const {
    return state() == Redo;
}

bool AceTreeTransaction::canUndo() const {
    return index() > 0;
}

bool AceTreeTransaction::canRedo() const {
    return index() < count();
}

#endif // CHORUSKIT_ACETREETRANSACTION_H
