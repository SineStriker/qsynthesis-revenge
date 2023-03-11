#include "LibraryLoadGuard.h"

#ifndef Q_OS_WINDOWS
LibraryLoadGuared::LibraryLoadGuared(const QString &path) {
}

LibraryLoadGuared::~LibraryLoadGuared() {
}
#else
#    include <Windows.h>

class LibraryLoadGuardPrivate {
public:
    QString path;
};

LibraryLoadGuard::LibraryLoadGuard(const QString &path) : d_ptr(new LibraryLoadGuardPrivate()) {
    wchar_t buf[MAX_PATH];
    if (::GetDllDirectoryW(MAX_PATH, buf)) {
        d_ptr->path = QString::fromStdWString(buf);
    }
    ::SetDllDirectoryW(path.toStdWString().data());
}

LibraryLoadGuard::~LibraryLoadGuard() {
    ::SetDllDirectoryW(d_ptr->path.isEmpty() ? NULL : d_ptr->path.toStdWString().data());
}

#endif
