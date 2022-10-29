#ifndef LVCOMMAND_H
#define LVCOMMAND_H

#include <QSharedPointer>
#include <QStringList>

class LVCommand {
public:
    LVCommand();
    virtual ~LVCommand();

    QString desc;
};

typedef QSharedPointer<LVCommand> QSCommandRef;
typedef QList<QSCommandRef> LVCommandList;

class ICommandSubscriber {
public:
    virtual void execute(const LVCommandList &cmds, bool isUndo) = 0;
};

#endif // LVCOMMAND_H
