#include "QVocaloidFile.h"

QVocaloidFile::QVocaloidFile(QVocaloidFile::Format format) : m_format(format) {
}

QVocaloidFile::QVocaloidFile() : m_format(VSQX) {
}

QVocaloidFile::Format QVocaloidFile::format() const {
    return m_format;
}

void QVocaloidFile::setFormat(const Format &format) {
    m_format = format;
}

bool QVocaloidFile::load(const QString &filename) {
    return true;
}

bool QVocaloidFile::save(const QString &filename) {
    return true;
}
