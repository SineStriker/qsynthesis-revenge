#ifndef QSLOCALEDIRPRIVATE_H
#define QSLOCALEDIRPRIVATE_H

#include "../QsLocaleDir.h"

class QSBASE_API QsLocaleDirPrivate {
    Q_DECLARE_PUBLIC(QsLocaleDir)
public:
    QsLocaleDirPrivate();
    virtual ~QsLocaleDirPrivate();

    void init();

    QsLocaleDir *q_ptr;

    QString dir;
    QString localeKey;
    
    struct RootItem {
        QString key;
        QString dir;
        QMap<QString, QStringList> files;
    };

    QMap<QString, RootItem> rootItems;
    bool loadRootItems(const QString &filename);
    
    void unloadLocale();
};

#endif // QSLOCALEDIRPRIVATE_H