#import <Cocoa/Cocoa.h>
#include "MacDarkTheme.h"

bool macDarkThemeAvailable() {
    if (__builtin_available(macOS 10.14, *)) {
        return true;
    } else {
        return false;
    }
}

bool macIsDarkTheme() {
    bool isDark = false;
    if (__builtin_available(macOS 10.14, *)) {
        NSAppearance *appearance = [NSAppearance currentAppearance];
        isDark = [appearance.name isEqualToString:NSAppearanceNameDarkAqua];
    }
    return isDark;
}
