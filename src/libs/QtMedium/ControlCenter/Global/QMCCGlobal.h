#ifndef QMCCGlobal_H
#define QMCCGlobal_H

#ifndef QMCC_API
#  ifdef QMCC_STATIC
#    define QMCC_API
#  else
#    ifdef QMCC_LIBRARY
#      define QMCC_API Q_DECL_EXPORT
#    else
#      define QMCC_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#define QMCC_BEGIN_NAMESPACE namespace QMControlCenter {
#define QMCC_END_NAMESPACE }

#endif // QMCCGlobal_H
