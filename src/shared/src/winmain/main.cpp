#include "shared_global.h"

#ifndef APP_ENABLE_ENTRY
#    include "utils_win.h"
#else
COM_EXTERN_C_IMPORT int main_entry(int, char *[]);
#endif

#include <iostream>

int main(int argc, char *argv[]) {
#ifdef APP_ENABLE_ENTRY
    return main_entry(argc, argv);
#else
    std::wstring exePath = WinGetExeDir();
    std::wstring binPath = exePath + TO_UNICODE("\\") + TO_UNICODE(APP_BIN_DIR);
    WinLibrary::AddLibDir(binPath); // Append subdirectory

    // Directory hints
    WinLibrary::SetEnv(L"QT_PLUGIN_PATH", binPath + L"\\" + L"plugins");
    WinLibrary::SetEnv(L"QTMEDIUM_BINARY_PATH", exePath + L"\\" + L"bin");

    // Load library
    WinLibrary winLib;
    if (!winLib.Load(binPath + TO_UNICODE("\\") + TO_UNICODE(APP_DLL))) {
        return winLib.ErrorCode();
    }

    // Goto entry
    typedef int (*EntryFun)(int, char *[]);
    auto entry = reinterpret_cast<EntryFun>(winLib.GetEntry("main_entry"));
    if (!entry) {
        return winLib.ErrorCode();
    }

    return entry(argc, argv);
#endif
}
