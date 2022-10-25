#ifndef QSCOMMAND_H
#define QSCOMMAND_H

#include <QSharedPointer>
#include <QStringList>

class QSCommand {
public:
    QSCommand();
    virtual ~QSCommand();

    QString desc;
};

typedef QSharedPointer<QSCommand> QSCommandRef;
typedef QList<QSCommandRef> QSCommandList;

class ICommandSubscriber {
public:
    virtual void execute(const QSCommandList &cmds, bool isUndo);
};

#endif // QSCOMMAND_H
