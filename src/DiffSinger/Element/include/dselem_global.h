#ifndef __DSELEM_GLOBAL_H__
#define __DSELEM_GLOBAL_H__

#include <QtGlobal>

#ifndef DSELEM_API
#  ifdef DSELEM_STATIC
#    define DSELEM_API
#  else
#    ifdef DSELEM_LIBRARY
#      define DSELEM_API Q_DECL_EXPORT
#    else
#      define DSELEM_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // __DSELEM_GLOBAL_H__