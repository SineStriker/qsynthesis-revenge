#include "QOtoIni.h"

#include "Config/UtaConstants.h"
#include "Utils/QUtaUtils.h"

#include "QMCodec.h"
#include "QMSystem.h"

using namespace Utau;

#include <QFile>

Q_CHARSET_DECLARE(QOtoIni)

static const char Slash = '/';

QOtoIni::QOtoIni() {
    QOtoIni::reset();
}

QOtoIni::~QOtoIni() {
}

bool QOtoIni::load(const QString &filename) {
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
        QTextCodec *codec = QMCodec::GetUtfCodec(data, &m_charsetDetermined);
        if (codec) {
            m_codec = codec;
        } else {
            m_codec = s_codeForDefault;
        }
    }
    in.setCodec(m_codec);

    QString line;

    while (!in.atEnd()) {
        line = in.readLine();
        if (line.isEmpty()) {
            continue;
        }
        QOtoItem genon = QUtaUtils::StringToGenon(line);
        QString filename = QMFs::PathFindDirPath(filename) + Slash + genon.FileName;

        auto it = OtoSamples.find(filename);
        if (it == OtoSamples.end()) {
            it = OtoSamples.insert(filename, {});
        }
        it.value().append(genon);
    }

    return true;
}

bool QOtoIni::save(const QString &filename) {
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
            out << QUtaUtils::GenonToString(*it) << Qt::endl;
        }
    }
    file.close();
    return true;
}

void QOtoIni::reset() {
    m_codec = nullptr;
    OtoSamples.clear();
}

bool QOtoIni::isEmpty() const {
    return OtoSamples.isEmpty();
}
