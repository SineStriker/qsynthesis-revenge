#ifndef QSSVS_GLOBAL_H
#define QSSVS_GLOBAL_H

#include <QtGlobal>

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

#endif // QSSVS_GLOBAL_H
