#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QMessageBox>
#include <QWidget>

class SectionNotes;

class FileParser : public QObject {
    Q_OBJECT
public:
    explicit FileParser(QWidget *parent = nullptr);
    ~FileParser();

    bool parseFile(const QString &filename, SectionNotes &notes);

protected:
    bool parseMidiFile(const QString &filename, SectionNotes &notes);        // *.mid
                                                                             //
    bool parseSynthVFile(const QString &filename, SectionNotes &notes);      // *.svp
    bool parseSynthVOldFile(const QString &filename, SectionNotes &notes);   // *.s5p
                                                                             //
    bool parseVocaloidFile(const QString &filename, SectionNotes &notes);    // *.vsqx
    bool parseVocaloidOldFile(const QString &filename, SectionNotes &notes); // *.vsq
                                                                             //
    bool parseOpenUTAUFile(const QString &filename, SectionNotes &notes);    // *.ustx

public:
    static bool isSuffixParsable(const QString &suffix);

signals:
};

#endif // FILEPARSER_H
