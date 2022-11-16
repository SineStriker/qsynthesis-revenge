#ifndef QSUNDOCOMMAND_H
#define QSUNDOCOMMAND_H

#include <QSharedPointer>
#include <QStringList>

#include "qsintegrate_global.h"

class QSINTEGRATE_API QsUndoCommand {
public:
    QsUndoCommand();
    virtual ~QsUndoCommand();

    QString desc;
};

typedef QSharedPointer<QsUndoCommand> QsUndoCommandRef;
typedef QList<QsUndoCommandRef> QsUndoCommandList;

class QSINTEGRATE_API QsCommandSubscriber {
public:
    virtual void execute(const QsUndoCommandList &cmds, bool isUndo) = 0;
};

#endif // QSUNDOCOMMAND_H
