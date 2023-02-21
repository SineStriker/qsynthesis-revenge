#ifndef QSFILEMANAGER_P_H
#define QSFILEMANAGER_P_H

#include "../QsFileManager.h"
#include "QsAbstractManager_p.h"
#include "QMFileSet.h"

#include <QHash>
#include <QMap>

class QSBASE_API QsFileManagerPrivate : public QsAbstractManagerPrivate {
    Q_DECLARE_PUBLIC(QsFileManager)
public:
    QsFileManagerPrivate();
    ~QsFileManagerPrivate();

    void init();

    struct RecentDesc {
        QString key;
        QMFileSet set;
    };
    QHash<int, RecentDesc> recentFileMap;
    int fileRegMax;
    int dirRegMax;

    QMap<QString, QString> lastOpenPaths;

    bool load_helper(const QString &filename);
    bool save_helper(const QString &filename);

    QString getLastOpenPath(const QString &type);
    void saveLastOpenDir(const QString &type, const QString &path, bool upper = true);
};

#endif // QSFILEMANAGER_P_H
