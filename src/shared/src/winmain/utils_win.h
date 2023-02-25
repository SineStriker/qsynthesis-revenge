#pragma once

#include "Windows.h"

#include <iostream>

// C:\Qt\5.15.2\Src\qtbase\src\winmain

extern int main(int, char *[]);

/*
  WinMain() - Initializes Windows and calls user's startup function main().
  NOTE: WinMain() won't be called if the application was linked as a "console"
  application.
*/
extern "C" int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR /*cmdParamarg*/, int /*cmdShow*/);

// Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::wstring WinGetLastErrorString(int *errNum = nullptr);

std::wstring WinGetExeDir();

class WinLibrary {
public:
    WinLibrary();
    ~WinLibrary();

    bool Load(const std::wstring &path);
    void Free();

    FARPROC GetEntry(const char *entry) const;
    int ErrorCode() const;

    static void AddLibDir(const std::wstring &path);
    static void SetEnv(const std::wstring &key, const std::wstring &val);

private:
    class Impl;
    Impl *_impl;
};