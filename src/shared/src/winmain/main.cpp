#include "com_global.h"

#if defined(_WIN32)
#    include "utils_win.h"
#endif

#ifdef APP_ENABLE_ENTRY
COM_EXTERN_C_IMPORT int main_entry(int, char *[]);
#endif

#include <iostream>

int main(int argc, char *argv[]) {
#ifdef APP_ENABLE_ENTRY
    return main_entry(argc, argv);
#else
    std::wstring path = WinGetExeDir() + TO_UNICODE("\\") + TO_UNICODE(APP_LIB_DIR);
    WinLibrary::AddLibDir(path); // Append subdirectory
    
    // Tell the library it's an external executable
    WinLibrary::SetEnv(TO_UNICODE("CHORUSKIT_EXTERN_WINMAIN"), TO_UNICODE("1"));
    
    // Tell QApplication another plugin path
    WinLibrary::SetEnv(TO_UNICODE("QT_PLUGIN_PATH"), path + TO_UNICODE("\\") + TO_UNICODE("plugins"));

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
