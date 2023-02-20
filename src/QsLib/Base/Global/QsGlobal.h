#ifndef QSGLOBAL_H
#define QSGLOBAL_H

#include <QtGlobal>

#ifndef QSBASE_API
#  ifdef QSBASE_STATIC
#    define QSBASE_API
#  else
#    ifdef QSBASE_LIBRARY
#      define QSBASE_API Q_DECL_EXPORT
#    else
#      define QSBASE_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#define QSAPI_BEGIN_NAMESPACE namespace QsApi {
#define QSAPI_END_NAMESPACE }
#define QSAPI_USING_NAMESPACE using namespace QsApi;

#endif // QSGLOBAL_H