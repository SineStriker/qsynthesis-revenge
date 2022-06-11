#ifndef SPECIALFILE_H
#define SPECIALFILE_H

#include <QDateTime>
#include <QString>

/*
 * Usage: a file util to read and write a file. The filename won't be lost after loading or saving.
 */

class SpecialFile {
public:
    SpecialFile();
    ~SpecialFile();

public:
    bool load(bool *valid = nullptr);
    bool save();

    bool remove();

    void clear();
    void reset();

public:
    QString filename() const;
    void setFilename(const QString &filename);

    bool exists() const;

    QDateTime lastModified() const;

protected:
    virtual bool loadCore(bool *valid);
    virtual bool saveCore();

    virtual void resetCore();
    virtual void prepareCore();

    QString m_filename;
    QDateTime m_lastModified;
};

#endif // SPECIALFILE_H
