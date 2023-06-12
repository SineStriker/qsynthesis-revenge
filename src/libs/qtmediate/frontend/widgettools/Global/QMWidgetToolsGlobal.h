#ifndef QMWIDGETTOOLSGLOBAL_H
#define QMWIDGETTOOLSGLOBAL_H

#include <QtGlobal>

#include "QMWidgetsGlobal.h"

#ifndef QMWTOOLS_EXPORT
#    ifdef QMWIDGETTOOLS_STATIC
#        define QMWTOOLS_EXPORT
#    else
#        ifdef QMWIDGETTOOLS_LIBRARY
#            define QMWTOOLS_EXPORT Q_DECL_EXPORT
#        else
#            define QMWTOOLS_EXPORT Q_DECL_IMPORT
#        endif
#    endif
#endif


#endif // QMWIDGETTOOLSGLOBAL_H
