#ifndef TEMPLATEGLOBAL_H
#define TEMPLATEGLOBAL_H

#include <QtGlobal>

#if defined(TEMPLATE_LIBRARY)
#    define TEMPLATE_EXPORT Q_DECL_EXPORT
#else
#    define TEMPLATE_EXPORT Q_DECL_IMPORT
#endif

#endif // TEMPLATEGLOBAL_H
