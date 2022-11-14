#ifndef QSFILENAMAGERPRIVATE_H
#define QSFILENAMAGERPRIVATE_H

#include "../QsFileManager.h"
#include "QsAbstractManager_p.h"
#include "Templates/QFileSet.h"

#include <QMap>

class QSINTEGRATE_API QsFileManagerPrivate : public QsAbstractManagerPrivate {
    Q_DECLARE_PUBLIC(QsFileManager)
public:
    QsFileManagerPrivate();
    ~QsFileManagerPrivate();

    void init();

    QFileSet projects;
    QFileSet folders;

    QMap<QString, QString> lastOpenPaths;

    QString getLastOpenPath(const QString &type);
    void saveLastOpenDir(const QString &type, const QString &path, bool upper = true);
};

#endif // QSFILENAMAGERPRIVATE_H
