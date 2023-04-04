#ifndef LIBRARYLOADGUARD_H
#define LIBRARYLOADGUARD_H

#include <QScopedPointer>
#include <QString>

class LibraryLoadGuardPrivate;

class LibraryLoadGuard {
public:
    LibraryLoadGuard(const QString &path);
    ~LibraryLoadGuard();

protected:
#ifdef Q_OS_WINDOWS
    QScopedPointer<LibraryLoadGuardPrivate> d_ptr;
#endif
};

#endif // LIBRARYLOADGUARD_H
