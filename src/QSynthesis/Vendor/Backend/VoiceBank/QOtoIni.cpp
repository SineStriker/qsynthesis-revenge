#include "QOtoIni.h"
#include "QUtauStrings.h"

#include "TextHelper.h"

using namespace Utau;

#include <QFile>

Q_CHARSET_DECLARE(QOtoIni)

QOtoIni::QOtoIni() {
    reset();
}

QOtoIni::QOtoIni(const QString &filename) {
    setDirname(filename);
}

QOtoIni::~QOtoIni() {
}

bool QOtoIni::loadCore(const QString &filename, bool *valid) {
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

    QString line;

    QString lastFilename;
    QOtoSample *lastSample = nullptr;

    while (!in.atEnd()) {
        line = in.readLine();
        if (line.isEmpty()) {
            continue;
        }
        QGenonSettings genon = Utau::StringToGenon(line);
        QString filename = m_dirname + '/' + genon.mFileName;
        if (filename == lastFilename && lastSample) {
            lastSample->append(genon);
            continue;
        }
        auto it = OtoSamples.find(filename);
        if (it == OtoSamples.end()) {
            it = OtoSamples.insert(filename, QOtoSample(filename));
            lastFilename = filename;
            lastSample = &it.value();
        }
    }

    return true;
}

bool QOtoIni::saveCore(const QString &filename) {
    QFile file(filename);

    // Delete when empty
    if (OtoSamples.isEmpty()) {
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

    for (auto it = OtoSamples.begin(); it != OtoSamples.end(); ++it) {
        const auto &sample = *it;
        for (auto it = sample.begin(); it != sample.end(); ++it) {
            out << Utau::GenonToString(*it) << Qt::endl;
        }
    }
    file.close();
    return true;
}

void QOtoIni::resetCore() {
    m_codec = nullptr;
    OtoSamples.clear();
}

QString QOtoIni::configName() const {
    return FILE_NAME_OTO_INI;
}
