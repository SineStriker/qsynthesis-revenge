#ifndef QSFRAMEWORKGLOBAL_H
#define QSFRAMEWORKGLOBAL_H

#include "QMWidgetsGlobal.h"
#include "QsGlobal.h"

#ifndef QSFRAMEWORK_API
#    ifdef QSFRAMEWORK_STATIC
#        define QSFRAMEWORK_API
#    else
#        ifdef QSFRAMEWORK_LIBRARY
#            define QSFRAMEWORK_API Q_DECL_EXPORT
#        else
#            define QSFRAMEWORK_API Q_DECL_IMPORT
#        endif
#    endif
#endif

#endif // QSFRAMEWORKGLOBAL_H
