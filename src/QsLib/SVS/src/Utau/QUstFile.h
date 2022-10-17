#ifndef QUSTFILE_H
#define QUSTFILE_H

#include <QString>

#include "QUstNote.h"
#include "QUstSettings.h"
#include "QUstVersion.h"

#include "qsutils_macros.h"

class QSSVS_API QUstFile {
    Q_CHARSET
public:
    QUstFile();
    ~QUstFile();

public:
    // Specify before loading or saving
    QString voiceDir;

    QUstVersion sectionVersion;
    QUstSettings sectionSettings;
    QList<QUstNote> sectionNotes;

public:
    bool load(const QString &filename);
    bool save(const QString &filename);

    void reset();

private:
    bool parseSectionName(const QString &str, QString &name);
    bool parseSectionNote(const QStringList &sectionList, QUstNote &note);
    bool parseSectionVersion(const QStringList &sectionList, QUstVersion &version);
    bool parseSectionSettings(const QStringList &sectionList, QUstSettings &setting);

    void writeSectionName(const int &name, QTextStream &out);
    void writeSectionName(const QString &name, QTextStream &out);

    void writeSectionNote(int num, const QUstNote &note, QTextStream &out);
    void writeSectionVersion(QTextStream &out);
    void writeSectionSettings(QTextStream &out);
};

#endif // QUSTFILE_H
