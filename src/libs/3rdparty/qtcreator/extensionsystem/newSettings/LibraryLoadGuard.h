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
};

#endif // LIBRARYLOADGUARD_H
