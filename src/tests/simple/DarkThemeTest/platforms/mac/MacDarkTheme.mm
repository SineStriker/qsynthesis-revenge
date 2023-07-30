#import <Cocoa/Cocoa.h>
#include "MacDarkTheme.h"

static id observer;

MacDarkTheme::MacDarkTheme(QObject *parent) : IDarkTheme(parent) {
    observer = [
        [NSDistributedNotificationCenter defaultCenter]
        addObserverForName: @"AppleInterfaceThemeChangedNotification"
                    object: nil
                     queue: [NSOperationQueue mainQueue]
                usingBlock: ^ (NSNotification *note) {
                    emitThemeChanged();
                }
    ];

}

MacDarkTheme::~MacDarkTheme() {
    [
        [NSDistributedNotificationCenter defaultCenter]
        removeObserver: observer
    ];
    observer = nil;
}

bool MacDarkTheme::isDarkTheme() {
    bool isDark = false;
    if (__builtin_available(macOS 10.14, *)) {
        NSString *osxMode = [[NSUserDefaults standardUserDefaults] stringForKey:@"AppleInterfaceStyle"];
        if (osxMode && [osxMode isEqualToString:@"Dark"]) {
            isDark = true;
        }
    }
    return isDark;
}

const char *MacDarkTheme::getOSName() {
    return "macOS";
}

void MacDarkTheme::emitThemeChanged() {
    bool isDark = isDarkTheme();
    emit themeChanged(isDark);
}
