#include <QtGlobal>

#ifndef FSNOTIFIER_API
#  ifdef FSNOTIFIER_STATIC
#    define FSNOTIFIER_API
#  else
#    ifdef FSNOTIFIER_LIBRARY
#      define FSNOTIFIER_API Q_DECL_EXPORT
#    else
#      define FSNOTIFIER_API Q_DECL_IMPORT
#    endif
#  endif
#endif