#include <Windows.h>

#include "WinDarkTheme.h"

// TODO: real time respond to dark mode change on Windows

WinDarkTheme::WinDarkTheme(QObject *parent) : IDarkTheme(parent) {
}

WinDarkTheme::~WinDarkTheme() {
}

bool WinDarkTheme::isDarkTheme() {
    bool isDark = false;

    HKEY key;
    DWORD value = 0;
    DWORD size = sizeof(value);

    // Open the registry key for the current user
    LONG result = RegOpenKeyExW(
        HKEY_CURRENT_USER,
        L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        0,
        KEY_READ,
        &key);
    if (result == ERROR_SUCCESS) {
        // Read the value of the AppsUseLightTheme key
        result = RegGetValueW(
            key,
            nullptr,
            L"AppsUseLightTheme",
            RRF_RT_REG_DWORD,
            nullptr,
            &value,
            &size);
        if (result == ERROR_SUCCESS) {
            // Check if the value is 0 (dark mode is enabled)
            isDark = (value == 0);
        } else {
            isDark = false;
        }

        RegCloseKey(key);
    } else {
        isDark = false;
    }

    return isDark;
}

const char *WinDarkTheme::getOSName() {
    return "Windows";
}

void WinDarkTheme::emitThemeChanged() {
    emit themeChanged(isDarkTheme());
}
