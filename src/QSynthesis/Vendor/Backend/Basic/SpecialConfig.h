#ifndef SPECIALCONFIG_H
#define SPECIALCONFIG_H

#include <QDateTime>
#include <QString>

/*
 * Usage: a file util to read and write a special file with a fixed name of a directory. The
 * dirname may be in need when parsing or writing the file, so it should be specified first.
 */

class SpecialConfig {
public:
    SpecialConfig();
    ~SpecialConfig();

public:
    bool load(bool *valid = nullptr);
    bool save();

    bool fromLocalFile(const QString &filename, bool *valid = nullptr);
    bool toLocalFile(const QString &filename);

    bool remove();

    void clear();
    void reset();

public:
    QString filename() const;

    QString dirname() const;
    void setDirname(const QString &dirname);

    bool exists() const;
    bool dirExists() const;

    QDateTime lastModified() const;

protected:
    virtual bool loadCore(const QString &filename, bool *valid);
    virtual bool saveCore(const QString &filename);

    virtual void resetCore();
    virtual void prepareCore();

    virtual QString configName() const;

    QString m_dirname;
    QDateTime m_lastModified;
};

#endif // SPECIALCONFIG_H
