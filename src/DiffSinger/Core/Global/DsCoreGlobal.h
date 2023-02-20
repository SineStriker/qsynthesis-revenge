#ifndef DSCOREGLOBAL_H
#define DSCOREGLOBAL_H

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

#define DSAPI_BEGIN_NAMESPACE namespace DsApi {
#define DSAPI_END_NAMESPACE }
#define DSAPI_USING_NAMESPACE using namespace DsApi;

#endif // DSCOREGLOBAL_H
