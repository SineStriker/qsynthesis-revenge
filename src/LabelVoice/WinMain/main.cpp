#if defined(_WIN32)

// C:\Qt\5.15.2\Src\qtbase\src\winmain

#include <windows.h>

#include <shellapi.h>

#define _TO_UNICODE(y) L##y
#define TO_UNICODE(x) _TO_UNICODE(x)

int main(int, char *[]);

/*
  WinMain() - Initializes Windows and calls user's startup function main().
  NOTE: WinMain() won't be called if the application was linked as a "console"
  application.
*/

// Convert a wchar_t to char string, equivalent to QString::toLocal8Bit()
// when passed CP_ACP.
static inline char *wideToMulti(unsigned int codePage, const wchar_t *aw) {
    const int required = WideCharToMultiByte(codePage, 0, aw, -1, nullptr, 0, nullptr, nullptr);
    char *result = new char[required];
    WideCharToMultiByte(codePage, 0, aw, -1, result, required, nullptr, nullptr);
    return result;
}

extern "C" int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR /*cmdParamarg*/, int /*cmdShow*/) {
    int argc = 0;
    wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (argvW == nullptr)
        return -1;
    char **argv = new char *[argc + 1];
    for (int i = 0; i != argc; ++i)
        argv[i] = wideToMulti(CP_ACP, argvW[i]);
    argv[argc] = nullptr;
    LocalFree(argvW);
    const int exitCode = main(argc, argv);
    for (int i = 0; (i != argc) && (argv[i] != nullptr); ++i)
        delete[] argv[i];
    delete[] argv;
    return exitCode;
}

#endif

#include <iostream>

#ifndef DELAY_LOAD
#include "lvstaging_global.h"
#endif

int main(int argc, char *argv[]) {
#ifdef _WIN32
    // Get executable file path
    std::wstring wstr;
    wchar_t buf[MAX_PATH + 1] = {0};
    if (::GetModuleFileNameW(NULL, buf, MAX_PATH) != 0) {
        wstr = buf;
    } else {
        ::exit(-1);
    }

    // Get executable directory
    size_t idx = wstr.find_last_of(L"\\");
    if (idx == std::wstring::npos) {
        ::MessageBoxW(nullptr, TO_UNICODE("Bad file path!"), TO_UNICODE("Error"),
                      MB_OK | MB_ICONERROR);
        return -1;
    }
    wstr.resize(idx);

    // Append subdirectory
    wstr += L"\\";
    wstr += TO_UNICODE(APP_LIB_DIR);
    ::SetDllDirectoryW(wstr.data());
#endif

#if defined(_WIN32) && defined(DELAY_LOAD)
    wstr += L"\\";
    wstr += TO_UNICODE(APP_DLL);

    typedef int (*EntryFun)(int, char *[]);

    HINSTANCE hDLL = ::LoadLibraryW(wstr.data());
    int res = -1;
    if (hDLL != NULL) {
        EntryFun fun = (EntryFun)::GetProcAddress(hDLL, "main_entry");
        if (fun != NULL) {
            res = fun(argc, argv);
        } else {
            res = ::GetLastError();
            ::MessageBoxW(nullptr, TO_UNICODE("Failed to find entry!"), TO_UNICODE("Error"),
                          MB_OK | MB_ICONERROR);
        }
        ::FreeLibrary(hDLL);
    } else {
        res = ::GetLastError();
        ::MessageBoxW(nullptr, TO_UNICODE("Failed to load main module!"), TO_UNICODE("Error"),
                      MB_OK | MB_ICONERROR);
    }
    return res;
#else
    return main_entry(argc, argv);
#endif
}
