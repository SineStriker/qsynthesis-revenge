#ifndef QMGLOBAL_H
#define QMGLOBAL_H

#include <QtGlobal>

#include "QMMacros.h"

#ifndef QMCORELIB_API
#  ifdef QMCORELIB_STATIC
#    define QMCORELIB_API
#  else
#    ifdef QMCORELIB_LIBRARY
#      define QMCORELIB_API Q_DECL_EXPORT
#    else
#      define QMCORELIB_API Q_DECL_IMPORT
#    endif
#  endif
#endif

void QMCORELIB_API QMCoreLib_Init();

#endif // QMGLOBAL_H
