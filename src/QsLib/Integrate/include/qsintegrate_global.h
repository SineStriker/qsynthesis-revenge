#ifndef QSINTEGRATE_GLOBAL_H
#define QSINTEGRATE_GLOBAL_H

#include <QtGlobal>

#ifndef QSINTEGRATE_API
#  ifdef QSINTEGRATE_STATIC
#    define QSINTEGRATE_API
#  else
#    ifdef QSINTEGRATE_LIBRARY
#      define QSINTEGRATE_API Q_DECL_EXPORT
#    else
#      define QSINTEGRATE_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // QSINTEGRATE_GLOBAL_H
