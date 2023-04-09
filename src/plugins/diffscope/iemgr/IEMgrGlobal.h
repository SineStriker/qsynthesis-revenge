#ifndef IEMGRGLOBAL_H
#define IEMGRGLOBAL_H

#include <QtGlobal>

#if defined(IEMGR_LIBRARY)
#    define IEMGR_EXPORT Q_DECL_EXPORT
#else
#    define IEMGR_EXPORT Q_DECL_IMPORT
#endif

#endif // IEMGRGLOBAL_H
