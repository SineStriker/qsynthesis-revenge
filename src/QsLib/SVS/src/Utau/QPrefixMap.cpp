#include "QPrefixMap.h"

#include "Config/UtaConstants.h"
#include "Utils/QUtaUtils.h"

#include "TextHelper.h"

using namespace Utau;

#include <QFile>
#include <QTextStream>

Q_CHARSET_DECLARE(QPrefixMap)

QPrefixMap::QPrefixMap() {
    QPrefixMap::reset();
}

QPrefixMap::~QPrefixMap() {
}

QString QPrefixMap::prefixedLyric(int noteNum, const QString &lyric) const {
    QString res = lyric;
    auto p = PrefixMap.find(noteNum);
    if (p != PrefixMap.end()) {
        res = p.value() + res;
    }
    p = SuffixMap.find(noteNum);
    if (p != SuffixMap.end()) {
        res += p.value();
    }
    return res;
}

bool QPrefixMap::load(const QString &filename) {
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

    int min = TONE_NUMBER_BASE;
    int max = min + (TONE_OCTAVE_MAX - TONE_OCTAVE_MIN + 1) * TONE_OCTAVE_STEPS - 1;

    QString line;
    QStringList lineStrings;

    while (!in.atEnd()) {
        line = in.readLine();
        if (!line.isEmpty()) {
            lineStrings = line.split("\t");
            if (lineStrings.size() >= 3) {
                int noteNum = QUtaUtils::ToneNameToToneNum(lineStrings[0]);
                if (noteNum >= min && noteNum <= max) {
                    PrefixMap[noteNum] = lineStrings[1];
                    SuffixMap[noteNum] = lineStrings[2];
                }
            }
        }
    }
    return true;
}

bool QPrefixMap::save(const QString &filename) {
    QFile file(filename);

    // Delete when empty
    if (isEmpty()) {
        if (file.exists()) {
            return file.remove();
        } else {
            return true;
        }
    }
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    if (!m_codec) {
        m_codec = s_codeForDefault;
    }
    out.setCodec(m_codec);

    for (auto it = PrefixMap.begin(); it != PrefixMap.end(); ++it) {
        int key = it.key();
        out << QUtaUtils::ToneNumToToneName(key) << '\t';
        out << PrefixMap[key] << "\t";
        out << SuffixMap[key] << Qt::endl;
    }

    file.close();
    return true;
}

void QPrefixMap::reset() {
    m_codec = nullptr;
    int min = TONE_NUMBER_BASE;
    int max = min + (TONE_OCTAVE_MAX - TONE_OCTAVE_MIN + 1) * TONE_OCTAVE_STEPS - 1;
    for (int i = min; i <= max; ++i) {
        PrefixMap[i] = QString();
        SuffixMap[i] = QString();
    }
}

bool QPrefixMap::isEmpty() const {
    bool res = true;
    for (auto it = PrefixMap.begin(); it != PrefixMap.end(); ++it) {
        if (!it.value().isEmpty()) {
            res = false;
            break;
        }
    }
    if (!res) {
        return false;
    }
    for (auto it = SuffixMap.begin(); it != SuffixMap.end(); ++it) {
        if (!it.value().isEmpty()) {
            res = false;
            break;
        }
    }
    return res;
}
