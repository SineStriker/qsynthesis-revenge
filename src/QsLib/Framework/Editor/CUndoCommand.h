#ifndef CUNDOCOMMAND_H
#define CUNDOCOMMAND_H

#include <QSharedPointer>
#include <QStringList>

#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API CUndoCommand {
public:
    CUndoCommand();
    virtual ~CUndoCommand();

    /**
     * @brief Description
     *
     */
    QString desc;
};

typedef QSharedPointer<CUndoCommand> CUndoCommandRef;
typedef QList<CUndoCommandRef> CUndoCommandList;

class QSFRAMEWORK_API CUndoSubscriber {
public:
    /**
     * @brief Inherit this class and implement this interface
     *
     * @param cmds Commands
     * @param isUndo Whether this action is undo or redo
     */
    virtual void execute(const CUndoCommandList &cmds, bool isUndo) = 0;
};

#endif // CUNDOCOMMAND_H
