#ifndef QMFILESET_H
#define QMFILESET_H

#include <QDir>
#include <QSet>

#include "QMGlobal.h"

/*
 * Usage: util to storage filename or dirname with the order of insertion time, and keep every
 * element unique.
 */

class QMCORELIB_API QMFileSet {
public:
    enum Type { File, Directory };

    QMFileSet();
    QMFileSet(Type type);

public:
    void set(const QStringList &files);

    QStringList all() const;
    QStringList valid() const;

    void filter();

    bool push(const QString &s);
    bool unshift(const QString &s);

    void advance(const QString &s);
    bool remove(const QString &s);

    bool isEmpty() const;
    bool contains(const QString &s);

    void clear();

    Type type() const;
    void setType(const Type &type);

protected:
    QStringList m_files;
    Type m_type;
};

#endif // QMFILESET_H
