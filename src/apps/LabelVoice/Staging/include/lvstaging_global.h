#ifndef LVSTAGING_GLOBAL_H
#define LVSTAGING_GLOBAL_H

#include <QtGlobal>

#ifndef LVSTAGING_API
#  ifdef LVSTAGING_STATIC
#    define LVSTAGING_API
#  else
#    ifdef LVSTAGING_LIBRARY
#      define LVSTAGING_API Q_DECL_EXPORT
#    else
#      define LVSTAGING_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // LVSTAGING_GLOBAL_H
