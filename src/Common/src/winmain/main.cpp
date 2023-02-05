#include "com_global.h"

#if defined(_WIN32)
#include "utils_win.h"
#endif

#ifdef APP_ENABLE_ENTRY
MY_EXTERN_C_IMPORT int main_entry(int, char *[]);
#endif

#include <iostream>

int main(int argc, char *argv[]) {
#ifdef _WIN32
    std::wstring path = WinGetExeDir() + TO_UNICODE("\\") + TO_UNICODE(APP_LIB_DIR);
    WinLibrary::AddLibDir(path); // Append subdirectory
#endif

#ifdef APP_ENABLE_ENTRY
    return main_entry(argc, argv);
#else
    // Load library
    WinLibrary winLib;
    if (!winLib.Load(path + TO_UNICODE("\\") + TO_UNICODE(APP_DLL))) {
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
