#ifndef LIBRARYLOADGUARD_H
#define LIBRARYLOADGUARD_H

#include <QString>
#include <QScopedPointer>

class LibraryLoadGuardPrivate;

class LibraryLoadGuard {
public:
    LibraryLoadGuard(const QString &path);
    ~LibraryLoadGuard();

protected:
    QScopedPointer<LibraryLoadGuardPrivate> d_ptr;
};

#endif // LIBRARYLOADGUARD_H
