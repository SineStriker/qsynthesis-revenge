#include "QReadmeText.h"
#include "QUtauBasic.h"
#include "QUtauStrings.h"

#include "TextHelper.h"

#include <QFile>
#include <QTextCodec>
#include <QTextStream>

using namespace Utau;

Q_CHARSET_STATIC_DECLARE(QReadmeText)

QReadmeText::QReadmeText() {
    reset();
}

QReadmeText::QReadmeText(const QString &dirname) {
    setDirname(dirname);
}

QReadmeText::~QReadmeText() {
}

bool QReadmeText::loadCore(const QString &filename, bool *valid) {
    Q_UNUSED(valid)

    QFile file(filename);
    if (!file.exists()) {
        return true;
    }
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QByteArray data;
    data = file.readAll();
    file.close();

    if (!m_codec) {
        // Detect Code
        QTextCodec *codec = Txt::GetUtfCodec(data, &m_charsetDetermined);
        if (codec) {
            m_codec = codec;
        } else {
            m_codec = s_codeForDefault;
        }
    }
    Text = m_codec->toUnicode(data);
    return true;
}

bool QReadmeText::saveCore(const QString &filename) {
    QFile file(filename);

    // Delete when empty
    if (Text.isEmpty()) {
        if (file.exists()) {
            return file.remove();
        } else {
            return true;
        }
    }

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    if (!m_codec) {
        m_codec = s_codeForDefault;
    }
    file.write(m_codec->fromUnicode(Text));
    file.close();
    return true;
}

void QReadmeText::resetCore() {
    m_codec = nullptr;
    Text.clear();
}

QString QReadmeText::configName() const {
    return FILE_NAME_VOICE_README;
}
