#ifndef VPIANOCOMMAND_H
#define VPIANOCOMMAND_H

#include "Commands/LVCommand.h"

class VPianoCommand : public LVCommand {
public:
    enum Type {
        ItemAdd,
        ItemRemove,
        ItemMove,
    };

    VPianoCommand(Type type);
    ~VPianoCommand();

    inline int type() const {
        return t;
    }

protected:
    int t;
};

#endif // VPIANOCOMMAND_H
