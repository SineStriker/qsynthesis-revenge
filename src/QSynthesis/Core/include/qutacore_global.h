#ifndef QUTACORE_GLOBAL_H
#define QUTACORE_GLOBAL_H

#include <QtGlobal>

#ifndef QUTACORE_API
#  ifdef QUTACORE_STATIC
#    define QUTACORE_API
#  else
#    ifdef QUTACORE_LIBRARY
#      define QUTACORE_API Q_DECL_EXPORT
#    else
#      define QUTACORE_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // QUTACORE_GLOBAL_H
