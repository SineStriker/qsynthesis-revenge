#ifndef DSMEDIUMGLOBAL_H
#define DSMEDIUMGLOBAL_H

#include <QtGlobal>

#ifndef DSMEDIUM_API
#  ifdef DSMEDIUM_STATIC
#    define DSMEDIUM_API
#  else
#    ifdef DSMEDIUM_LIBRARY
#      define DSMEDIUM_API Q_DECL_EXPORT
#    else
#      define DSMEDIUM_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // DSMEDIUMGLOBAL_H
