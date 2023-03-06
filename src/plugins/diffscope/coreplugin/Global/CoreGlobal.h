#ifndef COREGLOBAL_H
#define COREGLOBAL_H

#include <QtGlobal>

#if defined(CORE_LIBRARY)
#    define CORE_EXPORT Q_DECL_EXPORT
#else
#    define CORE_EXPORT Q_DECL_IMPORT
#endif

#endif // COREGLOBAL_H
