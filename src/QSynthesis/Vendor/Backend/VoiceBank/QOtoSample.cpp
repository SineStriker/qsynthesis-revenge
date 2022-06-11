#include "QOtoSample.h"
#include "SystemHelper.h"

#include <QDir>
#include <QFileInfo>
#include <QHash>

QOtoSample::QOtoSample() {
    clear();
}

QOtoSample::QOtoSample(const QString &filename) {
    setFilename(filename);
}

QOtoSample::~QOtoSample() {
}

QString QOtoSample::filename() const {
    return m_filename;
}

void QOtoSample::setFilename(const QString &filename) {
    clear();
    m_filename = filename;
}

void QOtoSample::clearAll() {
    clear();
    m_filename.clear();
}

bool QOtoSample::valid() const {
    return Sys::isFileExist(m_filename);
}

bool QOtoSample::frqExist() const {
    return Sys::isFileExist(Utau::toFrqName(m_filename));
}

bool QOtoSample::mrqExist() const {
    return false;
}

bool QOtoSample::operator==(const QOtoSample &sample) const {
    return m_filename == sample.m_filename;
}

bool QOtoSample::operator!=(const QOtoSample &sample) const {
    return !((*this) == sample);
}

bool QOtoSample::operator<(const QOtoSample &sample) const {
    return m_filename.toLower() < sample.m_filename.toLower();
}

uint qHash(const QOtoSample &samples, uint seed) {
    return qHash(samples.m_filename, seed);
}
