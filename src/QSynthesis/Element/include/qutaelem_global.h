#ifndef __QUTAELEM_GLOBAL_H__
#define __QUTAELEM_GLOBAL_H__

#include <QtGlobal>

#ifndef QUTAELEM_API
#  ifdef QUTAELEM_STATIC
#    define QUTAELEM_API
#  else
#    ifdef QUTAELEM_LIBRARY
#      define QUTAELEM_API Q_DECL_EXPORT
#    else
#      define QUTAELEM_API Q_DECL_IMPORT
#    endif
#  endif
#endif


#endif // __QUTAELEM_GLOBAL_H__