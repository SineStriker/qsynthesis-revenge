#ifndef DSHOSTGLOBAL_H
#define DSHOSTGLOBAL_H

#include <QtGlobal>

#ifndef DSHOST_API
#  ifdef DSHOST_STATIC
#    define DSHOST_API
#  else
#    ifdef DSHOST_LIBRARY
#      define DSHOST_API Q_DECL_EXPORT
#    else
#      define DSHOST_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // DSHOSTGLOBAL_H
