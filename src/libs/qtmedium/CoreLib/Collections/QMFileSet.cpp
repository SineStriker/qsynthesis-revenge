#include "QMFileSet.h"

#include <QDir>

#define _FIN(s) QDir::fromNativeSeparators(s)

QMFileSet::QMFileSet() : m_type(File) {
}

QMFileSet::QMFileSet(QMFileSet::Type type) : m_type(type) {
}

QMFileSet::Type QMFileSet::type() const {
    return m_type;
}

void QMFileSet::setType(const Type &type) {
    m_type = type;
}

void QMFileSet::set(const QStringList &files) {
    m_files.clear();
    for (auto it = files.begin(); it != files.end(); ++it) {
        push(*it);
    }
}

QStringList QMFileSet::all() const {
    return m_files;
}

QStringList QMFileSet::valid() const {
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

void QMFileSet::filter() {
    set(valid());
}

bool QMFileSet::unshift(const QString &s) {
    QString filename = _FIN(s);
    int index = m_files.indexOf(filename);
    if (index >= 0) {
        return false;
    }
    m_files.prepend(filename);
    return true;
}

void QMFileSet::advance(const QString &s) {
    QString filename = _FIN(s);
    remove(filename);
    unshift(filename);
}

bool QMFileSet::push(const QString &s) {
    QString filename = _FIN(s);
    int index = m_files.indexOf(filename);
    if (index >= 0) {
        return false;
    }
    m_files.append(filename);
    return true;
}

bool QMFileSet::remove(const QString &s) {
    QString filename = _FIN(s);
    int index = m_files.indexOf(filename);
    if (index < 0) {
        return false;
    }
    m_files.removeAt(index);
    return true;
}

bool QMFileSet::isEmpty() const {
    return m_files.isEmpty();
}

bool QMFileSet::contains(const QString &s) {
    return m_files.contains(s);
}

void QMFileSet::clear() {
    m_files.clear();
}
