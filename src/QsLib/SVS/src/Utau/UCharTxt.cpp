#include "UCharTxt.h"

#include "Serialization/QSettingFile.h"
#include "TextHelper.h"

#include "Config/UtaCharText.h"

#include <QDir>
#include <QFile>
#include <QTextStream>

using namespace Utau;

Q_CHARSET_DECLARE(UCharTxt)

UCharTxt::UCharTxt() {
    UCharTxt::reset();
}

UCharTxt::~UCharTxt() {
}

bool UCharTxt::load(const QString &filename) {
    QFile file(filename);
    QByteArray data;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    data = file.readAll();
    file.close();

    QTextStream in(&data);
    // Detect Code
    QTextCodec *codec = Txt::GetUtfCodec(data, &m_charsetDetermined);
    if (codec) {
        m_codec = codec;
    } else if (!m_codec) {
        m_codec = s_codeForDefault;
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
        name = *nameValue;
    }
    QString *avatarValue = section.valueOf(KEY_NAME_CHAR_IMAGE);
    if (avatarValue) {
        avatar = QDir::fromNativeSeparators(*avatarValue);
    }
    QString *authorValue = section.valueOf(KEY_NAME_CHAR_AUTHOR);
    if (authorValue) {
        author = *authorValue;
    }
    QString *spriteValue = section.valueOf(KEY_NAME_CHAR_FOREGROUND);
    if (spriteValue) {
        sprite = QDir::fromNativeSeparators(*spriteValue);
    }
    custom = section.unformattedLines();

    return true;
}

bool UCharTxt::save(const QString &filename) {
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
    if (!name.isEmpty()) {
        section.addPair(KEY_NAME_CHAR_NAME, name);
    }
    if (!avatar.isEmpty()) {
        section.addPair(KEY_NAME_CHAR_IMAGE, QDir::toNativeSeparators(avatar));
    }
    if (!author.isEmpty()) {
        section.addPair(KEY_NAME_CHAR_AUTHOR, author);
    }
    if (!sprite.isEmpty()) {
        section.addPair(KEY_NAME_CHAR_FOREGROUND, QDir::toNativeSeparators(sprite));
    }
    section.setUnformattedLines(custom);

    QStringList lines = section.toLines();

    out.setCodec(m_codec);
    for (auto it = lines.begin(); it != lines.end(); ++it) {
        out << *it << Qt::endl;
    }

    file.close();
    return true;
}

void UCharTxt::reset() {
    m_codec = nullptr;

    name.clear();
    author.clear();
    avatar.clear();
    sprite.clear();
}

bool UCharTxt::isEmpty() const {
    return name.isEmpty() && author.isEmpty() && avatar.isEmpty();
}
