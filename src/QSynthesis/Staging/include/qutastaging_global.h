#ifndef __QUTASTAGING_GLOBAL_H__
#define __QUTASTAGING_GLOBAL_H__

#include <QtGlobal>

#ifndef QUTASTAGING_API
#  ifdef QUTASTAGING_STATIC
#    define QUTASTAGING_API
#  else
#    ifdef QUTASTAGING_LIBRARY
#      define QUTASTAGING_API Q_DECL_EXPORT
#    else
#      define QUTASTAGING_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // __QUTASTAGING_GLOBAL_H__