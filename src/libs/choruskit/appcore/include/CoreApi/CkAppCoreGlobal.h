#ifndef CKAPPCOREGLOBAL_H
#define CKAPPCOREGLOBAL_H

#include <QtGlobal>

#ifndef CKAPPCORE_API
#    ifdef CKAPPCORE_STATIC
#        define CKAPPCORE_API
#    else
#        ifdef CKAPPCORE_LIBRARY
#            define CKAPPCORE_API Q_DECL_EXPORT
#        else
#            define CKAPPCORE_API Q_DECL_IMPORT
#        endif
#    endif
#endif


#endif // CKAPPCOREGLOBAL_H
