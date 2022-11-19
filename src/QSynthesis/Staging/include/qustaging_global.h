#ifndef __QUSTAGING_GLOBAL_H__
#define __QUSTAGING_GLOBAL_H__

#include <QtGlobal>

#ifndef QUSTAGING_API
#  ifdef QUSTAGING_STATIC
#    define QUSTAGING_API
#  else
#    ifdef QUSTAGING_LIBRARY
#      define QUSTAGING_API Q_DECL_EXPORT
#    else
#      define QUSTAGING_API Q_DECL_IMPORT
#    endif
#  endif
#endif

#endif // __QUSTAGING_GLOBAL_H__