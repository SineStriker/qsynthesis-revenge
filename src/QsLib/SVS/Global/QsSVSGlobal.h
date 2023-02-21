#ifndef QSSVSGLOBAL_H
#define QSSVSGLOBAL_H

#include "QsGlobal.h"

#ifndef QSSVS_API
#  ifdef QSSVS_STATIC
#    define QSSVS_API
#  else
#    ifdef QSSVS_LIBRARY
#      define QSSVS_API Q_DECL_EXPORT
#    else
#      define QSSVS_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // QSSVSGLOBAL_H
