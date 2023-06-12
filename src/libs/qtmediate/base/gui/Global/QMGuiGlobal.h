#ifndef QMGUIGLOBAL_H
#define QMGUIGLOBAL_H

#include <QtGlobal>

#include "QMGlobal.h"

#ifndef QMGUI_EXPORT
#    ifdef QMGUI_STATIC
#        define QMGUI_EXPORT
#    else
#        ifdef QMGUI_LIBRARY
#            define QMGUI_EXPORT Q_DECL_EXPORT
#        else
#            define QMGUI_EXPORT Q_DECL_IMPORT
#        endif
#    endif
#endif


#endif // QMGUIGLOBAL_H
