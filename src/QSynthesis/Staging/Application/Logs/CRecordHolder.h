#ifndef CRECORDHOLDER_H
#define CRECORDHOLDER_H

#include <QObject>

#include "CRecordData.h"
#include "FileHolder.h"
#include "Macros.h"

#define qRecord CRecordHolder::instance()
#define qRecordCData qRecord->constData()
#define qRecordData qRecord->data()

class CRecordHolder : public FileHolder {
    Q_OBJECT
    Q_SINGLETON(CRecordHolder)
public:
    explicit CRecordHolder(QObject *parent = nullptr);
    explicit CRecordHolder(const QString &filename, QObject *parent = nullptr);

    QString filename() const;
    void setFilename(const QString &filename);

public:
    bool load();
    bool save();

    void reset();

    const CRecordData &constData() const;
    CRecordData &data();

    // Request recent files by following methods instead
    enum RecentType { Project, Folder, Wavtool, Resampler };
    enum ChangeType { Push, Unshift, Advance, Remove, Clear };

    void commitRecent(RecentType rType, ChangeType cType, const QString &filename = "");
    QStringList fetchRecent(RecentType rType) const;

protected:
    CRecordData m_data;
    QString m_filename;

signals:
    void recentCommited(CRecordHolder::RecentType rType);
};

#endif // CRECORDHOLDER_H
