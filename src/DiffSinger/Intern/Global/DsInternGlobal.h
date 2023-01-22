#ifndef DSINTERNGLOBAL_H
#define DSINTERNGLOBAL_H

#include <QtGlobal>

#ifndef DSINTERN_API
#  ifdef DSINTERN_STATIC
#    define DSINTERN_API
#  else
#    ifdef DSINTERN_LIBRARY
#      define DSINTERN_API Q_DECL_EXPORT
#    else
#      define DSINTERN_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // DSINTERNGLOBAL_H
