#ifndef QSFRAMEWORK_GLOBAL_H
#define QSFRAMEWORK_GLOBAL_H

#include <QtGlobal>

#ifndef QSFRAMEWORK_API
#  ifdef QSFRAMEWORK_STATIC
#    define QSFRAMEWORK_API
#  else
#    ifdef QSFRAMEWORK_LIBRARY
#      define QSFRAMEWORK_API Q_DECL_EXPORT
#    else
#      define QSFRAMEWORK_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // QSFRAMEWORK_GLOBAL_H
