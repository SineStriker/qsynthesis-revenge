#ifndef QSLOCALEDIR_P_H
#define QSLOCALEDIR_P_H

#include "../QsLocaleDir.h"

class QMCORELIB_API QsLocaleDirPrivate {
    Q_DECLARE_PUBLIC(QsLocaleDir)
public:
    QsLocaleDirPrivate();
    virtual ~QsLocaleDirPrivate();

    void init();

    QsLocaleDir *q_ptr;

    struct RootItem {
        QString key;
        QString dir;
        QMap<QString, QStringList> files;
    };

    QString dir;
    
    QString version;
    QString parent;
    
    QList<RootItem> locales;

    virtual bool loadNext(const QJsonObject &objDoc);
    virtual void unloadNext();

    void unloadLocale();

    static bool loadRootItems(const QJsonObject &obj, RootItem *out);
};

#endif // QSLOCALEDIR_P_H
