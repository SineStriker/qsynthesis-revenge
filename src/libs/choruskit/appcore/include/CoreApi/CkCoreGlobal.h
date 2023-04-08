#ifndef CKCOREGLOBAL_H
#define CKCOREGLOBAL_H

#include <QtGlobal>

#ifndef CKCORE_API
#    ifdef CKCORE_STATIC
#        define CKCORE_API
#    else
#        ifdef CKCORE_LIBRARY
#            define CKCORE_API Q_DECL_EXPORT
#        else
#            define CKCORE_API Q_DECL_IMPORT
#        endif
#    endif
#endif


#endif // CKCOREGLOBAL_H
