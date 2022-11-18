#if defined(_WIN32)

// C:\Qt\5.15.2\Src\qtbase\src\winmain

#include <windows.h>

#include <shellapi.h>

#include <iostream>

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

// Returns the last Win32 error, in string format. Returns an empty string if there is no error.
std::wstring GetLastErrorAsString() {
    // Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::wstring(); // No error message has been recorded
    }

    LPWSTR messageBuffer = nullptr;

    // Ask Win32 to give us the string version of that message ID.
    // The parameters we pass in, tell Win32 to create the buffer that holds the message for us
    // (because we don't yet know how long the message string will be).
    size_t size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                     FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                 (LPWSTR) &messageBuffer, 0, NULL);

    // Copy the error message into a std::string.
    std::wstring message(messageBuffer, size);

    // Free the Win32's string's buffer.
    LocalFree(messageBuffer);

    return message;
}

#endif

#ifdef APP_ENABLE_ENTRY
#include "shared.h"
#ifdef APP_ENABLE_C_STYLE
extern "C" {
#endif

DECL_IMPORT int main_entry(int, char *[]);

#ifdef APP_ENABLE_C_STYLE
}
#endif
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

    // Record original error mode
    UINT prevErrorMode = ::GetErrorMode();
    ::SetErrorMode(0);

    HINSTANCE hDLL = ::LoadLibraryW(wstr.data());
    int res = -1;
    if (hDLL != NULL) {
        EntryFun fun = (EntryFun)::GetProcAddress(hDLL, "main_entry");
        if (fun != NULL) {
            // Restore error mode
            SetErrorMode(prevErrorMode);

            res = fun(argc, argv);
        } else {
            res = ::GetLastError();

            std::wstring msg = GetLastErrorAsString();
            ::MessageBoxW(nullptr, msg.data(), TO_UNICODE("Error"), MB_OK | MB_ICONERROR);
        }
        ::FreeLibrary(hDLL);
    } else {
        res = ::GetLastError();

        std::wstring msg = GetLastErrorAsString();
        ::MessageBoxW(nullptr, msg.data(), TO_UNICODE("Error"), MB_OK | MB_ICONERROR);
    }
    return res;
#else
    return main_entry(argc, argv);
#endif
}
