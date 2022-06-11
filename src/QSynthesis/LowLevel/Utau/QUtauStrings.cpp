#include "QUtauStrings.h"
#include "Common/QUtauStd.h"
#include "Common/QUtauStd_p.h"

using namespace Utau;

#include <QDir>

#include <cstring>

QString fromUSTVoiceDir(const QString &filePath, const QString &appPath) {
    QString path = QDir::fromNativeSeparators(filePath);
    QString voice = appPath + SLASH + DIR_NAME_VOICE + SLASH;

    if (path.startsWith(DIR_VOICE_BASE)) {
        path = voice + path.mid(int(strlen(DIR_VOICE_BASE)));
    } else if (QFileInfo(path).isRelative()) {
        path = appPath + SLASH + path;
    }

    return path;
}

QString toUSTVoiceDir(const QString &filePath, const QString &appPath) {
    QString path = filePath;
    QString voice = appPath + SLASH + DIR_NAME_VOICE + SLASH;

    if (path.startsWith(voice)) {
        path = DIR_VOICE_BASE + path.mid(voice.size());
    }

    //    path = QDir::toNativeSeparators(path);
    return path;
}

QString fromUSTToolsDir(const QString &filePath, const QString &appPath) {
    QString path = QDir::fromNativeSeparators(filePath);
    if (path.startsWith("./") || path.startsWith(".\\")) {
        path.remove(0, 2);
    }
    if (QDir::isRelativePath(path)) {
        path = appPath + SLASH + path;
    }
    return path;
}

QString toUSTToolsDir(const QString &filePath, const QString &appPath) {
    QString path = filePath;
    QString app = appPath + SLASH;
    if (filePath.startsWith(app)) {
        path = path.mid(app.size());
    }
    //    path = QDir::toNativeSeparators(path);
    return path;
}
