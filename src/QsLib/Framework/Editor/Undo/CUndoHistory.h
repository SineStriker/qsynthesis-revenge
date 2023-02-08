#ifndef QSEDITORHISTORY_H
#define QSEDITORHISTORY_H

#include <QObject>
#include <QSet>

#include "CUndoCommand.h"

class QSFRAMEWORK_API CUndoHistory : public QObject {
    Q_OBJECT
public:
    explicit CUndoHistory(QObject *parent = nullptr);
    ~CUndoHistory();

public:
    void saveNow();
    bool isEdited() const;

    bool canUndo() const;
    bool canRedo() const;

    void undo();
    void redo();

    void execute(const CUndoCommandRef &cmd, bool isUndo);
    void execute(const CUndoCommandList &cmds, bool isUndo);

    void startRecord();
    void stopRecord(const QString &desc);

    void addSubscriber(CUndoSubscriber *subscriber);
    void removeSubscriber(CUndoSubscriber *subscriber);

protected:
    QList<CUndoCommandList> historyStack;
    int historyIndex;
    int savedHistoryIndex;
    QSet<CUndoSubscriber *> subscribers;

    // temp
    bool recording;
    CUndoCommandList recordList;

    bool edited;

    void setEdited(bool edited);

signals:
    void editStateChanged(bool saved);
};

#endif // QSEDITORHISTORY_H
