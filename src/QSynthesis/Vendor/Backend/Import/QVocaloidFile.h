#ifndef QVOCALOIDFILE_H
#define QVOCALOIDFILE_H

#include <QObject>
#include <QXmlStreamReader>

/*
 * Usage: util to parse or write Vocaloid project file(.vsqx .vsq).
 */
class QVocaloidFile {
public:
    enum Format {
        VSQ,
        VSQX,
    };

    QVocaloidFile(Format format);
    QVocaloidFile();

public:
    Format format() const;
    void setFormat(const Format &format);

    bool load(const QString &filename);
    bool save(const QString &filename);

private:
    Format m_format;
};

#endif // QVOCALOIDFILE_H
