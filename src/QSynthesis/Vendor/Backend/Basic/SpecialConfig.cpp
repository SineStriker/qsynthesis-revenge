#include "SpecialConfig.h"
#include "SystemHelper.h"

using namespace Sys;

#include <QFile>

static const char Slash = '/';

SpecialConfig::SpecialConfig() {
}

SpecialConfig::~SpecialConfig() {
}

bool SpecialConfig::load(bool *valid) {
    QString filename = this->filename();
    bool res = fromLocalFile(filename, valid);
    if (res) {
        m_lastModified = QFileInfo(filename).lastModified().toLocalTime();
    }
    return res;
}

bool SpecialConfig::save() {
    QString filename = this->filename();
    bool res = toLocalFile(filename);
    if (res) {
        m_lastModified = QFileInfo(filename).lastModified().toLocalTime();
    }
    return res;
}

bool SpecialConfig::fromLocalFile(const QString &filename, bool *valid) {
    resetCore();
    return loadCore(filename, valid);
}

bool SpecialConfig::toLocalFile(const QString &filename) {
    return saveCore(filename);
}

bool SpecialConfig::remove() {
    QFile file(m_dirname);
    if (file.exists()) {
        return file.remove();
    } else {
        return true;
    }
}

void SpecialConfig::clear() {
    resetCore();
}

void SpecialConfig::reset() {
    m_dirname.clear();
    m_lastModified = QDateTime();
    resetCore();
}

QString SpecialConfig::filename() const {
    return m_dirname + Slash + configName();
}

QString SpecialConfig::dirname() const {
    return m_dirname;
}

void SpecialConfig::setDirname(const QString &dirname) {
    reset();
    m_dirname = dirname;
    prepareCore();
}

bool SpecialConfig::exists() const {
    return isFileExist(filename());
}

bool SpecialConfig::dirExists() const {
    return isDirExist(m_dirname);
}

QDateTime SpecialConfig::lastModified() const {
    return m_lastModified;
}

bool SpecialConfig::loadCore(const QString &filename, bool *valid) {
    Q_UNUSED(filename)
    Q_UNUSED(valid)
    return false;
}

bool SpecialConfig::saveCore(const QString &filename) {
    Q_UNUSED(filename)
    return false;
}

void SpecialConfig::resetCore() {
}

void SpecialConfig::prepareCore() {
}

QString SpecialConfig::configName() const {
    return QString();
}
