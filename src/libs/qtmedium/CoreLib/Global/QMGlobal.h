#ifndef QMGLOBAL_H
#define QMGLOBAL_H

#include <QtGlobal>

#include "QMMacros.h"

#ifndef QMCORELIB_API
#  ifdef QMCORELIB_STATIC
#    define QMCORELIB_API
#  else
#    ifdef QMCORELIB_LIBRARY
#      define QMCORELIB_API Q_DECL_EXPORT
#    else
#      define QMCORELIB_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#define QM_BEGIN_NAMESPACE namespace Qm {
#define QM_END_NAMESPACE }

#ifndef QTMEDIUM_DISABLE_DEBUG
#    define qmDebug() qDebug().noquote().nospace()
#else
#    define qmDebug() QT_NO_QDEBUG_MACRO()
#endif

#endif // QMGLOBAL_H
