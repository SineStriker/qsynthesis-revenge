#ifndef QMEDITINGGLOBAL_H
#define QMEDITINGGLOBAL_H

#include <QtGlobal>

#include "QMWidgetsGlobal.h"

#ifndef QMEDITING_EXPORT
#    ifdef QMEDITING_STATIC
#        define QMEDITING_EXPORT
#    else
#        ifdef QMEDITING_LIBRARY
#            define QMEDITING_EXPORT Q_DECL_EXPORT
#        else
#            define QMEDITING_EXPORT Q_DECL_IMPORT
#        endif
#    endif
#endif


#endif // QMEDITINGGLOBAL_H
