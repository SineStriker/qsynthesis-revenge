#include "DefaultDarkTheme.h"

DefaultDarkTheme::DefaultDarkTheme(QObject *parent) : IDarkTheme(parent) {
}

DefaultDarkTheme::~DefaultDarkTheme() {
}

bool DefaultDarkTheme::isDarkTheme() {
    return false;
}

const char *DefaultDarkTheme::getOSName() {
    return "Other System";
}

void DefaultDarkTheme::emitThemeChanged() {
    emit themeChanged(false);
}
