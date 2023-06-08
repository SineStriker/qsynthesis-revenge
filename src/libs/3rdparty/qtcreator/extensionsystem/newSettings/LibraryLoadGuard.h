#ifndef LIBRARYLOADGUARD_H
#define LIBRARYLOADGUARD_H

#include <QPluginLoader>
#include <QString>

class LibraryLoadGuardPrivate;

class LibraryLoadGuard {
public:
    explicit LibraryLoadGuard(QPluginLoader *loader);
    ~LibraryLoadGuard();

protected:
    LibraryLoadGuardPrivate *d;

    Q_DISABLE_COPY_MOVE(LibraryLoadGuard);
};

#endif // LIBRARYLOADGUARD_H
