#ifndef QSUTILS_GLOBAL_H
#define QSUTILS_GLOBAL_H

#include <QtGlobal>

#ifndef QSUTILS_API
#  ifdef QSUTILS_STATIC
#    define QSUTILS_API
#  else
#    ifdef QSUTILS_LIBRARY
#      define QSUTILS_API Q_DECL_EXPORT
#    else
#      define QSUTILS_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // QSUTILS_GLOBAL_H
