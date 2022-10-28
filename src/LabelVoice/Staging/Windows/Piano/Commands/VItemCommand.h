#ifndef VITEMCOMMAND_H
#define VITEMCOMMAND_H

#include "VPianoCommand.h"

#include <QSharedPointer>

class VitemCommand : public VPianoCommand {
public:
    VitemCommand(Type type);
    ~VitemCommand();

public:
    struct ItemInfo {
        QString id;
        QString speaker;
    };

    struct VirtualDir {
        QString id;
        QSharedPointer<VirtualDir> subdirs;
    };
};

#endif // VITEMCOMMAND_H
