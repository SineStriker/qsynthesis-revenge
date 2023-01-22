#include "UPart.h"

#include <QFileInfo>

UPart::UPart() {
    UPart::clear();
}

UPart::~UPart() {
}

void UPart::clear() {
    name = "New Part";
    comment.clear();
    trackNo = 0;
    position = 0;
}

QString UPart::displayName() const {
    return QString();
}

int UPart::duration() const {
    return 0;
}

UVoicePart::UVoicePart() {
    UVoicePart::clear();
}

UVoicePart::~UVoicePart() {
}

void UVoicePart::clear() {
    UPart::clear();
    notes.clear();
    curves.clear();
}

QString UVoicePart::displayName() const {
    return name;
}

UWavePart::UWavePart() {
    UWavePart::clear();
}

UWavePart::~UWavePart() {
}

void UWavePart::clear() {
    UPart::clear();
    _filePath.clear();
    _missing = false;
    _duration = 0;

    relativePath.clear();
    fileDurationMs = 0;
    skipMs = 0;
    TrimMs = 0;

    peaks.clear();
    samples.clear();
    channels = 0;
    fileDurTick = 0;
}

QString UWavePart::filePath() const {
    return _filePath;
}

void UWavePart::setFilePath(const QString &path) {
    _filePath = path;
    name = QFileInfo(path).fileName();
}

QString UWavePart::displayName() const {
    return _missing ? QObject::tr("[Missing] %1").arg(name) : name;
}

int UWavePart::duration() const {
    return fileDurTick;
}
