#ifndef LVCORE_GLOBAL_H
#define LVCORE_GLOBAL_H

#include <QtGlobal>

#ifndef LVCORE_API
#  ifdef LVCORE_STATIC
#    define LVCORE_API
#  else
#    ifdef LVCORE_LIBRARY
#      define LVCORE_API Q_DECL_EXPORT
#    else
#      define LVCORE_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // LVCORE_GLOBAL_H
