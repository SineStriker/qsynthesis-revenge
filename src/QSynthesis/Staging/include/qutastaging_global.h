#ifndef QUTASTAGING_GLOBAL_H
#define QUTASTAGING_GLOBAL_H

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

#endif // QUTASTAGING_GLOBAL_H