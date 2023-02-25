#ifndef QMLOCALEDIR_P_H
#define QMLOCALEDIR_P_H

#include "../QMLocaleDir.h"

class QMCORELIB_API QMLocaleDirPrivate {
    Q_DECLARE_PUBLIC(QMLocaleDir)
public:
    QMLocaleDirPrivate();
    virtual ~QMLocaleDirPrivate();

    void init();

    QMLocaleDir *q_ptr;

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

#endif // QMLOCALEDIR_P_H
