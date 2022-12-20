#ifndef DSCORE_GLOBAL_H
#define DSCORE_GLOBAL_H

#include <QtGlobal>

#ifndef DSCORE_API
#  ifdef DSCORE_STATIC
#    define DSCORE_API
#  else
#    ifdef DSCORE_LIBRARY
#      define DSCORE_API Q_DECL_EXPORT
#    else
#      define DSCORE_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // DSCORE_GLOBAL_H
