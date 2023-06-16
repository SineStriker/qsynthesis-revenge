#include "LibraryLoadGuard.h"

#ifdef Q_OS_WINDOWS
#    include <Windows.h>
#else
#endif

class LibraryLoadGuardPrivate {
public:
    QPluginLoader *loader;
#ifdef Q_OS_WINDOWS
    QString path;
#endif

    explicit LibraryLoadGuardPrivate(QPluginLoader *loader) : loader(loader) {
    }
};

LibraryLoadGuard::LibraryLoadGuard(QPluginLoader *loader) : d(new LibraryLoadGuardPrivate(loader)) {
#ifdef Q_OS_WINDOWS
    wchar_t buf[MAX_PATH];
    if (::GetDllDirectoryW(MAX_PATH, buf)) {
        d->path = QString::fromStdWString(buf);
    }
    ::SetDllDirectoryW(loader->fileName().toStdWString().data());
#else
    loader->setLoadHints(loader->loadHints() | QLibrary::ExportExternalSymbolsHint);
#endif
}

LibraryLoadGuard::~LibraryLoadGuard() {
#ifdef Q_OS_WINDOWS
    ::SetDllDirectoryW(d->path.isEmpty() ? nullptr : d->path.toStdWString().data());
#endif
    delete d;
}