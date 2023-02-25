#ifndef QMWIDGETSGLOBAL_H
#define QMWIDGETSGLOBAL_H

#include <QtGlobal>

#include "QMGlobal.h"
#include "QMWidgetsMacros.h"

#ifndef QMWIDGETS_API
#    ifdef QMWIDGETS_STATIC
#        define QMWIDGETS_API
#    else
#        ifdef QMWIDGETS_LIBRARY
#            define QMWIDGETS_API Q_DECL_EXPORT
#        else
#            define QMWIDGETS_API Q_DECL_IMPORT
#        endif
#    endif
#endif

#endif // QMWIDGETSGLOBAL_H
