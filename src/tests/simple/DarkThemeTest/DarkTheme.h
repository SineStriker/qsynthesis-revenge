#ifndef CHORUSKIT_DARKTHEME_H
#define CHORUSKIT_DARKTHEME_H

#include <qglobal.h>

#if defined(Q_OS_MAC)
#include "platforms/mac/MacDarkTheme.h"
using DarkTheme = MacDarkTheme;
#elif defined(Q_OS_WIN)
#include "platforms/win/WinDarkTheme.h"
using DarkTheme = WinDarkTheme;
#else
#include "platforms/default/DefaultDarkTheme.h"
using DarkTheme = DefaultDarkTheme;
#endif

#endif // CHORUSKIT_DARKTHEME_H
