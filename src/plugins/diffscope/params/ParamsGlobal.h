#ifndef PARAMSGLOBAL_H
#define PARAMSGLOBAL_H

#include <QtGlobal>

#if defined(PARAMS_LIBRARY)
#    define PARAMS_EXPORT Q_DECL_EXPORT
#else
#    define PARAMS_EXPORT Q_DECL_IMPORT
#endif

#endif // PARAMSGLOBAL_H
