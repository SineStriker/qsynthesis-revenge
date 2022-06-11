#include "QSynthVFile.h"

QSynthVFile::QSynthVFile(QSynthVFile::Format format) : m_format(format) {
    m_format = format;
}

QSynthVFile::QSynthVFile() : m_format(SVP) {
}

QSynthVFile::Format QSynthVFile::format() const {
    return m_format;
}

void QSynthVFile::setFormat(const Format &format) {
    m_format = format;
}

bool QSynthVFile::load(const QString &filename) {
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return 0;
    }

    QString fileContent = file.readAll();
    QVector<double> tempoValueArr, tempoTimeArr;

    QJsonParseError aError;
    QByteArray aBytes = fileContent.toUtf8();
    QJsonDocument aRootDoc = QJsonDocument::fromJson(aBytes, &aError);

    if (aError.error != QJsonParseError::NoError) {
        return 0; // Wrong file format
    }
    if (aRootDoc.isObject()) {
        QJsonObject aRootObj = aRootDoc.object();

        // Get time information
        if (aRootObj.contains("time")) {
            QJsonValue aTime = aRootObj.value("time");
            if (aTime.isArray()) {
                QJsonArray aTimeArr = aTime.toArray();
                for (int i = 0; i < aTimeArr.size(); ++i) {
                    double aTempo = 120.0;
                    double aTime = 0;
                    QJsonValue aTimeVal = aTimeArr.at(i);

                    if (aTimeVal.isObject()) {
                        QJsonObject aTimeObj = aTimeVal.toObject();
                        if (aTimeObj.contains("position")) {
                            aTime = aTimeObj.value("position").toDouble(aTime);
                        }
                        if (aTimeObj.contains("bpm")) {
                            aTime = aTimeObj.value("bpm").toDouble(aTempo);
                        }
                    }
                }
            }
        }

        // Get notes
        if (aRootObj.contains("library")) {
            QJsonValue aLibrary = aRootObj.value("notes");
            if (aLibrary.isArray()) {
                QJsonArray aLibraryArr = aLibrary.toArray();
            }
        }
    }

    file.close();
    return true;
}
bool QSynthVFile::save(const QString &filename) {
    QFile file(filename);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return 0;
    }

    file.close();
    return true;
}
