#include "QLVItem.h"

bool LVModel::SliceDefinition::operator<(const LVModel::SliceDefinition &other) const {
    if (In < other.In) {
        return true;
    }
    return Out < other.Out;
}

