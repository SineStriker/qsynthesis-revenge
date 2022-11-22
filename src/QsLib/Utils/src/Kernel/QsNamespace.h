#ifndef QSNAMESPACE_H
#define QSNAMESPACE_H

#include "qsutils_global.h"

namespace Qs {

    enum Direction { Forward, Backward };

    enum Priority { Primary, Secondary };

    QSUTILS_API void Register();

}; // namespace Qs

#endif // QSNAMESPACE_H
