#include <qglobal.h>

#include "DarkTheme.h"

#ifdef Q_OS_MAC
#include "mac/MacDarkTheme.h"
#endif

bool isDarkTheme() {
    bool result = false;
#ifdef Q_OS_MAC
    result = macIsDarkTheme();
#endif
    return result;
}