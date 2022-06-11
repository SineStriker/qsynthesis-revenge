#include "SpecialFile.h"
#include "SystemHelper.h"

using namespace Sys;

#include <QFile>

SpecialFile::SpecialFile() {
}

SpecialFile::~SpecialFile() {
}

bool SpecialFile::load(bool *valid) {
    resetCore();
    if (valid) {
        *valid = true;
    }
    bool res = loadCore(valid);
    if (res) {
        m_lastModified = QFileInfo(m_filename).lastModified().toLocalTime();
    }
    return res;
}

bool SpecialFile::save() {
    bool res = saveCore();
    if (res) {
        m_lastModified = QFileInfo(m_filename).lastModified().toLocalTime();
    }
    return res;
}

bool SpecialFile::remove() {
    QFile file(m_filename);
    if (file.exists()) {
        return file.remove();
    } else {
        return true;
    }
}

void SpecialFile::clear() {
    resetCore();
}

void SpecialFile::reset() {
    m_filename.clear();
    m_lastModified = QDateTime();
    resetCore();
}

QString SpecialFile::filename() const {
    return m_filename;
}

void SpecialFile::setFilename(const QString &filename) {
    reset();
    m_filename = filename;
    prepareCore();
}

bool SpecialFile::exists() const {
    return isFileExist(m_filename);
}

QDateTime SpecialFile::lastModified() const {
    return m_lastModified;
}

bool SpecialFile::loadCore(bool *valid) {
    Q_UNUSED(valid)
    return false;
}

bool SpecialFile::saveCore() {
    return false;
}

void SpecialFile::resetCore() {
}

void SpecialFile::prepareCore() {
}
