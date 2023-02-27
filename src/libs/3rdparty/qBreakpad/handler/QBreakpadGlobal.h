#ifndef QBREAKPADGLOBAL_H
#define QBREAKPADGLOBAL_H

#ifndef QBREAKPAD_API
#    ifdef QBREAKPAD_STATIC
#        define QBREAKPAD_API
#    else
#        ifdef QBREAKPAD_LIBRARY
#            define QBREAKPAD_API Q_DECL_EXPORT
#        else
#            define QBREAKPAD_API Q_DECL_IMPORT
#        endif
#    endif
#endif

#endif // QBREAKPADGLOBAL_H
