#ifndef SEQUENCETEXTFILE_H
#define SEQUENCETEXTFILE_H

#include "Macros.h"

#include "QUtauBasic.h"
#include "QUtauSections.h"
#include "QUtauStrings.h"

#include "SpecialFile.h"

#include <QFile>
#include <QTextCodec>

/*
 * Usage: util to parse or write UTAU script file(.ust).
 */

class SequenceTextFile : public SpecialFile {
    Q_CHARSET
public:
    SequenceTextFile();
    SequenceTextFile(const QString &filename);
    ~SequenceTextFile();

    SectionVersion sectionVersion;
    SectionSettings sectionSettings;
    QList<QLinkNote> sectionNotes;

protected:
    bool loadCore(bool *valid) override;
    bool saveCore() override;
    void resetCore() override;

    bool parseSectionName(const QString &str, QString &name);
    bool parseSectionNote(const QStringList &sectionList, QLinkNote &note);
    bool parseSectionVersion(const QStringList &sectionList, SectionVersion &version);
    bool parseSectionSettings(const QStringList &sectionList, SectionSettings &setting);

    void writeSectionName(const int &name, QTextStream &out);
    void writeSectionName(const QString &name, QTextStream &out);

    void writeSectionNote(int num, const QLinkNote &note, QTextStream &out);
    void writeSectionVersion(QTextStream &out);
    void writeSectionSettings(QTextStream &out);
};

#endif // SEQUENCETEXTFILE_H
