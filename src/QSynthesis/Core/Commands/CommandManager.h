#ifndef COMMANDMANAGER_H
#define COMMANDMANAGER_H

#include <QObject>
#include <QSet>

#include "QSCommand.h"

class CommandManager : public QObject {
    Q_OBJECT
public:
    explicit CommandManager(QObject *parent = nullptr);
    ~CommandManager();

public:
    void undo();
    void redo();

    void execute(const QSCommand &cmd, bool isUndo);
    void execute(const QSCommandList &cmds, bool isUndo);

    void startRecord();
    void stopRecord();

    void addSubscriber(ICommandSubscriber *subscriber);
    void removeSubscriber(ICommandSubscriber *subscriber);

protected:
    QList<QSCommandList> historyStack;
    int historyIndex;
    int savedHistoryIndex;
    QSet<ICommandSubscriber *> subscribers;

    // temp
    bool recording;
    QSCommandList curCmdList;

signals:
};

#endif // COMMANDMANAGER_H
