#pragma once

#include <qglobal.h>

#ifndef QMIDI_API
#  ifdef QMIDI_STATIC
#    define QMIDI_API
#  else
#    ifdef QMIDI_LIBRARY
#      define QMIDI_API Q_DECL_EXPORT
#    else
#      define QMIDI_API Q_DECL_IMPORT
#    endif
#  endif
#endif
