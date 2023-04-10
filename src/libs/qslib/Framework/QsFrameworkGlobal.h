#ifndef QSFRAMEWORKGLOBAL_H
#define QSFRAMEWORKGLOBAL_H

#include "QMWidgetsGlobal.h"

#ifndef QSFRAMEWORK_API
#    ifdef QSFRAMEWORK_STATIC
#        define QSFRAMEWORK_API
#    else
#        ifdef QSFRAMEWORK_LIBRARY
#            define QSFRAMEWORK_API Q_DECL_EXPORT
#        else
#            define QSFRAMEWORK_API Q_DECL_IMPORT
#        endif
#    endif
#endif

#define QSAPI_BEGIN_NAMESPACE namespace QsApi {
#define QSAPI_END_NAMESPACE   }
#define QSAPI_USING_NAMESPACE using namespace QsApi;

#endif // QSFRAMEWORKGLOBAL_H
