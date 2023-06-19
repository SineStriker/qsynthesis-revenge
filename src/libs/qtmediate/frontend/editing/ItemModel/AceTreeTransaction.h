#ifndef CHORUSKIT_ACETREETRANSACTION_H
#define CHORUSKIT_ACETREETRANSACTION_H

#include <QMDisplayString.h>

#include "AceTreeModel.h"

class AceTreeTransactionPrivate;

class QMEDITING_EXPORT AceTreeTransaction : public QObject {
    Q_OBJECT
public:
    explicit AceTreeTransaction(AceTreeModel *model, QObject *parent = nullptr);
    ~AceTreeTransaction();

    bool isValid() const;

public:
    using FormatDescription = QString (*)(const QMap<QString, QString> &);

    static bool hasDescriptionFormatter(const QString &key);
    static void registerDescriptionFormatter(const QString &key, FormatDescription func);

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

    /**
     * Start a transaction, all changes will be recorded
     */
    void beginTransaction();

    /**
     * Abort current transaction
     */
    void abortTransaction();

    /**
     * Commit current transaction
     *
     * @param key Description formatter key
     * @param args Arguments to format description
     */
    void endTransaction(const QString &key, const QMap<QString, QString> &args);

    /*
     * Undo stack size
     */
    int count() const;

    /*
     * Operation index
     */
    int index() const;

    QString description(int index) const;
    inline QString currentUndoDescription() const;
    inline QString currentRedoDescription() const;

    void undo();
    void redo();

    inline bool canUndo() const;
    inline bool canRedo() const;

signals:
    void indexChanged(int index);

private:
    AceTreeTransactionPrivate *d;
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

QString AceTreeTransaction::currentUndoDescription() const {
    return description(index() - 1);
}

QString AceTreeTransaction::currentRedoDescription() const {
    return description(index());
}

bool AceTreeTransaction::canUndo() const {
    return index() > 0;
}

bool AceTreeTransaction::canRedo() const {
    return index() < count();
}

#endif // CHORUSKIT_ACETREETRANSACTION_H
