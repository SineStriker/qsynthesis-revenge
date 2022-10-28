#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QSet>

#include "LVCommand.h"

class CommandManager : public QObject {
    Q_OBJECT
public:
    explicit CommandManager(QObject *parent = nullptr);
    ~CommandManager();

public:
    void saveNow();
    bool isEdited() const;

    bool canUndo() const;
    bool canRedo() const;

    void undo();
    void redo();

    void execute(const QSCommandRef &cmd, bool isUndo);
    void execute(const LVCommandList &cmds, bool isUndo);

    void startRecord();
    void stopRecord(const QString &desc);

    void addSubscriber(ICommandSubscriber *subscriber);
    void removeSubscriber(ICommandSubscriber *subscriber);

protected:
    QList<LVCommandList> historyStack;
    int historyIndex;
    int savedHistoryIndex;
    QSet<ICommandSubscriber *> subscribers;

    // temp
    bool recording;
    LVCommandList recordList;

    bool edited;

    void setEdited(bool edited);

signals:
    void editStateChanged(bool saved);
};

#endif // COMMANDMANAGER_H
