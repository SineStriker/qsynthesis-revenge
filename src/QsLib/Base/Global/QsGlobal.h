#ifndef QSGLOBAL_H
#define QSGLOBAL_H

#include <QtGlobal>

#ifndef QSBASE_API
#  ifdef QSBASE_STATIC
#    define QSBASE_API
#  else
#    ifdef QSBASE_LIBRARY
#      define QSBASE_API Q_DECL_EXPORT
#    else
#      define QSBASE_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // QSGLOBAL_H