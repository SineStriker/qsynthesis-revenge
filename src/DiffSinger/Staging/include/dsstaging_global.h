#ifndef DSSTAGING_GLOBAL_H
#define DSSTAGING_GLOBAL_H

#include <QtGlobal>

#ifndef DSSTAGING_API
#  ifdef DSSTAGING_STATIC
#    define DSSTAGING_API
#  else
#    ifdef DSSTAGING_LIBRARY
#      define DSSTAGING_API Q_DECL_EXPORT
#    else
#      define DSSTAGING_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // DSSTAGING_GLOBAL_H
