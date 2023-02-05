#ifndef DSGUIGLOBAL_H
#define DSGUIGLOBAL_H

#include <QtGlobal>

#ifndef DSGUI_API
#  ifdef DSGUI_STATIC
#    define DSGUI_API
#  else
#    ifdef DSGUI_LIBRARY
#      define DSGUI_API Q_DECL_EXPORT
#    else
#      define DSGUI_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // DSGUIGLOBAL_H
