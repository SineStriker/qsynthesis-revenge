#include "utils_win.h"

#include "global.h"

static wchar_t Error_Title[] = TO_UNICODE("Fatal Error");

// Convert a wchar_t to char string, equivalent to QString::toLocal8Bit()
// when passed CP_ACP.
static char *wideToMulti(unsigned int codePage, const wchar_t *aw) {
    const int required = WideCharToMultiByte(codePage, 0, aw, -1, nullptr, 0, nullptr, nullptr);
    char *result = new char[required];
    WideCharToMultiByte(codePage, 0, aw, -1, result, required, nullptr, nullptr);
    return result;
}

int WinMain(HINSTANCE, HINSTANCE, LPSTR /*cmdParamarg*/, int /*cmdShow*/) {
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

std::wstring WinGetLastErrorString() {
    // Get the error message ID, if any.
    DWORD errorMessageID = ::GetLastError();
    if (errorMessageID == 0) {
        return std::wstring(); // No error message has been recorded
    }

    LPWSTR messageBuffer = nullptr;

    // Ask Win32 to give us the string version of that message ID.
    // The parameters we pass in, tell Win32 to create the buffer that holds the message for us
    // (because we don't yet know how long the message string will be).
    size_t size = ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                                       FORMAT_MESSAGE_IGNORE_INSERTS,
                                   NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                   (LPWSTR) &messageBuffer, 0, NULL);

    // Copy the error message into a std::string.
    std::wstring message(messageBuffer, size);

    // Free the Win32's string's buffer.
    ::LocalFree(messageBuffer);

    return message;
}

std::wstring WinGetExeDir() {
    // Get executable full path
    std::wstring wstr;
    wchar_t buf[MAX_PATH + 1] = {0};
    if (::GetModuleFileNameW(NULL, buf, MAX_PATH) != 0) {
        wstr = buf;
    } else {
        ::MessageBoxW(nullptr, TO_UNICODE("Failed to get module path!"), Error_Title,
                      MB_OK | MB_ICONERROR);
        ::exit(-1);
    }

    // Get executable directory
    size_t idx = wstr.find_last_of(L"\\");
    if (idx == std::wstring::npos) {
        ::MessageBoxW(nullptr, TO_UNICODE("Bad file path!"), Error_Title, MB_OK | MB_ICONERROR);
        ::exit(-1);
    }
    wstr.resize(idx);

    return wstr;
}

class WinLibrary::Impl {
public:
    Impl() {
        hDll = nullptr;
    }

    HMODULE hDll;
};

WinLibrary::WinLibrary() : _impl(new Impl()) {
}

WinLibrary::~WinLibrary() {
    Free();
    delete _impl;
}

bool WinLibrary::Load(const std::wstring &path) {
    HINSTANCE hDLL = ::LoadLibraryW(path.data());
    if (!hDLL) {
        std::wstring msg = WinGetLastErrorString();
        ::MessageBoxW(nullptr, msg.data(), Error_Title,
                      MB_OK | MB_TOPMOST | MB_SETFOREGROUND | MB_ICONERROR);
        return false;
    }
    _impl->hDll = hDLL;
    return true;
}

void WinLibrary::Free() {
    if (_impl->hDll) {
        ::FreeLibrary(_impl->hDll);
        _impl->hDll = nullptr;
    }
}

FARPROC WinLibrary::GetEntry(const char *entry) const {
    auto fun = ::GetProcAddress(_impl->hDll, entry);
    if (!fun) {
        std::wstring msg = WinGetLastErrorString();
        ::MessageBoxW(nullptr, msg.data(), Error_Title,
                      MB_OK | MB_TOPMOST | MB_SETFOREGROUND | MB_ICONERROR);
        return nullptr;
    }
    return fun;
}

int WinLibrary::ErrorCode() const {
    return ::GetLastError();
}

void WinLibrary::AddLibDir(const std::wstring &path) {
    ::SetDllDirectoryW(path.data());
}