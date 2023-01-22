#ifndef QSEDITORHISTORY_H
#define QSEDITORHISTORY_H

#include <QObject>
#include <QSet>

#include "QsCommand.h"

class QSINTEGRATE_API QsEditorHistory : public QObject {
    Q_OBJECT
public:
    explicit QsEditorHistory(QObject *parent = nullptr);
    ~QsEditorHistory();

public:
    void saveNow();
    bool isEdited() const;

    bool canUndo() const;
    bool canRedo() const;

    void undo();
    void redo();

    void execute(const QsUndoCommandRef &cmd, bool isUndo);
    void execute(const QsUndoCommandList &cmds, bool isUndo);

    void startRecord();
    void stopRecord(const QString &desc);

    void addSubscriber(QsCommandSubscriber *subscriber);
    void removeSubscriber(QsCommandSubscriber *subscriber);

protected:
    QList<QsUndoCommandList> historyStack;
    int historyIndex;
    int savedHistoryIndex;
    QSet<QsCommandSubscriber *> subscribers;

    // temp
    bool recording;
    QsUndoCommandList recordList;

    bool edited;

    void setEdited(bool edited);

signals:
    void editStateChanged(bool saved);
};

#endif // QSEDITORHISTORY_H
