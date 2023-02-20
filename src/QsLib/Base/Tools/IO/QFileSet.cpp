#include "QFileSet.h"

#include <QDir>

#define _FIN(s) QDir::fromNativeSeparators(s)

QFileSet::QFileSet() : m_type(File) {
}

QFileSet::QFileSet(QFileSet::Type type) : m_type(type) {
}

QFileSet::Type QFileSet::type() const {
    return m_type;
}

void QFileSet::setType(const Type &type) {
    m_type = type;
}

void QFileSet::set(const QStringList &files) {
    m_files.clear();
    for (auto it = files.begin(); it != files.end(); ++it) {
        push(*it);
    }
}

QStringList QFileSet::all() const {
    return m_files;
}

QStringList QFileSet::valid() const {
    QStringList files;
    for (auto it = m_files.begin(); it != m_files.end(); ++it) {
        const QString &cur = *it;
        QFileInfo info(cur);
        if (info.isRelative()) {
            continue;
        }
        if ((m_type == File && info.isFile()) || (m_type == Directory && info.isDir())) {
            files.append(cur);
        }
    }
    return files;
}

void QFileSet::filter() {
    set(valid());
}

bool QFileSet::unshift(const QString &s) {
    QString filename = _FIN(s);
    int index = m_files.indexOf(filename);
    if (index >= 0) {
        return false;
    }
    m_files.prepend(filename);
    return true;
}

void QFileSet::advance(const QString &s) {
    QString filename = _FIN(s);
    remove(filename);
    unshift(filename);
}

bool QFileSet::push(const QString &s) {
    QString filename = _FIN(s);
    int index = m_files.indexOf(filename);
    if (index >= 0) {
        return false;
    }
    m_files.append(filename);
    return true;
}

bool QFileSet::remove(const QString &s) {
    QString filename = _FIN(s);
    int index = m_files.indexOf(filename);
    if (index < 0) {
        return false;
    }
    m_files.removeAt(index);
    return true;
}

bool QFileSet::isEmpty() const {
    return m_files.isEmpty();
}

bool QFileSet::contains(const QString &s) {
    return m_files.contains(s);
}

void QFileSet::clear() {
    m_files.clear();
}
