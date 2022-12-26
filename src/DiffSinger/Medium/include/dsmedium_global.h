#ifndef DSMEDIUM_GLOBAL_H
#define DSMEDIUM_GLOBAL_H

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

#endif // DSMEDIUM_GLOBAL_H
