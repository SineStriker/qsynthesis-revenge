#ifndef LVELEM_GLOBAL_H
#define LVELEM_GLOBAL_H

#include <QtGlobal>

#ifndef LVELEM_API
#  ifdef LVELEM_STATIC
#    define LVELEM_API
#  else
#    ifdef LVELEM_LIBRARY
#      define LVELEM_API Q_DECL_EXPORT
#    else
#      define LVELEM_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // LVELEM_GLOBAL_H
