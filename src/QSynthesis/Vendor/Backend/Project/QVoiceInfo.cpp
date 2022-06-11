#include "QVoiceInfo.h"
#include "QSettingFile.h"

#include "SystemHelper.h"
#include "TextHelper.h"

#include "QUtauBasic.h"
#include "QUtauSections.h"
#include "QUtauStrings.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

using namespace Utau;

Q_CHARSET_DECLARE(QVoiceInfo)

QVoiceInfo::QVoiceInfo() {
    reset();
}

QVoiceInfo::QVoiceInfo(const QString &dirname) {
    setDirname(dirname);
}

QVoiceInfo::~QVoiceInfo() {
}

bool QVoiceInfo::loadCore(const QString &filename, bool *valid) {
    Q_UNUSED(valid)

    QFile file(filename);
    QByteArray data;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    data = file.readAll();
    file.close();

    QTextStream in(&data);
    if (!m_codec) {
        // Detect Code
        QTextCodec *codec = Txt::GetUtfCodec(data, &m_charsetDetermined);
        if (codec) {
            m_codec = codec;
        } else {
            m_codec = s_codeForDefault;
        }
    }
    in.setCodec(m_codec);

    QStringList lines;
    while (!in.atEnd()) {
        lines.append(in.readLine());
    }

    QSettingSection section;
    section.fromLines(lines);
    QString *nameValue = section.valueOf(KEY_NAME_CHAR_NAME);
    if (nameValue) {
        m_name = *nameValue;
    }
    QString *avatarValue = section.valueOf(KEY_NAME_CHAR_IMAGE);
    if (avatarValue) {
        m_avatar = QDir::fromNativeSeparators(*avatarValue);
    }
    QString *authorValue = section.valueOf(KEY_NAME_CHAR_AUTHOR);
    if (authorValue) {
        m_author = *authorValue;
    }
    QString *spriteValue = section.valueOf(KEY_NAME_CHAR_FOREGROUND);
    if (spriteValue) {
        m_sprite = QDir::fromNativeSeparators(*spriteValue);
    }
    m_custom = section.unformattedLines();

    return true;
}

bool QVoiceInfo::saveCore(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    if (!m_codec) {
        m_codec = s_codeForDefault;
    }
    out.setCodec(m_codec);

    QSettingSection section;
    if (!m_name.isEmpty()) {
        section.addPair(KEY_NAME_CHAR_NAME, m_name);
    }
    if (!m_avatar.isEmpty()) {
        section.addPair(KEY_NAME_CHAR_IMAGE, QDir::toNativeSeparators(m_avatar));
    }
    if (!m_author.isEmpty()) {
        section.addPair(KEY_NAME_CHAR_AUTHOR, m_author);
    }
    if (!m_sprite.isEmpty()) {
        section.addPair(KEY_NAME_CHAR_FOREGROUND, QDir::toNativeSeparators(m_sprite));
    }
    section.setUnformattedLines(m_custom);

    QStringList lines = section.toLines();

    out.setCodec(m_codec);
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        out << *it << Qt::endl;
    }

    file.close();
    return true;
}

QString QVoiceInfo::name() const {
    return m_name;
}

QString QVoiceInfo::title() const {
    if (m_name.isEmpty()) {
        return QDir::toNativeSeparators(Sys::PathFindFileName(dirname()));
    } else {
        return m_name;
    }
}

QString QVoiceInfo::author() const {
    return m_author;
}

QString QVoiceInfo::avatar() const {
    return m_avatar;
}

QString QVoiceInfo::sprite() const {
    return m_sprite;
}

void QVoiceInfo::setName(const QString &name) {
    m_name = name;
}

void QVoiceInfo::setAuthor(const QString &author) {
    m_author = author;
}

void QVoiceInfo::setAvatar(const QString &avatar) {
    m_avatar = avatar;
}

void QVoiceInfo::setSprite(const QString &sprite) {
    m_sprite = sprite;
}

void QVoiceInfo::resetCore() {
    m_codec = nullptr;

    m_name.clear();
    m_author.clear();
    m_avatar.clear();
    m_sprite.clear();
}

QString QVoiceInfo::configName() const {
    return FILE_NAME_CHAR_TEXT;
}
