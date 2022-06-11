#ifndef QSYNTHVFILE_H
#define QSYNTHVFILE_H

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QObject>

/*
 * Usage: util to parse or write Synthesiszer V project file(.svp .s5p).
 */
class QSynthVFile {
public:
    enum Format {
        S5P,
        SVP,
    };

    struct Tempo {
        double pos;
        double bpm;

        Tempo() {
            pos = 0.0;
            bpm = 120.0;
        }
    };

    struct Note {
        qint64 onset;
        qint64 duration;
        int pitch;
        QString lyric;

        Note() {
            duration = 1;
            onset = 0;
            pitch = 60;
            lyric = "la";
        }
    };

    QSynthVFile(Format format);
    QSynthVFile();

public:
    Format format() const;
    void setFormat(const Format &format);

    bool load(const QString &filename);
    bool save(const QString &filename);

private:
    Format m_format;
};

#endif // QSYNTHVFILE_H
