#ifndef QSFILENAMAGERPRIVATE_H
#define QSFILENAMAGERPRIVATE_H

#include "../QsFileManager.h"
#include "QsAbstractManager_p.h"
#include "Tools/QFileSet.h"

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
        QFileSet set;
    };
    QHash<int, RecentDesc> recentMap;
    int fileRegMax;
    int dirRegMax;

    QMap<QString, QString> lastOpenPaths;

    bool load_helper(const QString &filename);
    bool save_helper(const QString &filename);

    QString getLastOpenPath(const QString &type);
    void saveLastOpenDir(const QString &type, const QString &path, bool upper = true);
};

#endif // QSFILENAMAGERPRIVATE_H
