#ifndef QSLOCALEDIR_H
#define QSLOCALEDIR_H

#include "QSimpleVarExp.h"
#include "QsGlobal.h"

#include <QJsonObject>

class QSBASE_API QsLocaleDir {
public:
    QsLocaleDir();
    explicit QsLocaleDir(const QString &dir);
    virtual ~QsLocaleDir();

public:
    QString dir;
    QSimpleVarExp vars;

    QString localeKey;

    void setDir(const QString &dir);
    bool loadDefault(const QString &binName);

    virtual bool load(const QString &filename);
    void unload();

protected:
    struct RootItem {
        QString key;
        QString dir;
        QMap<QString, QStringList> files;
    };

    QMap<QString, RootItem> rootItems;
    bool loadRootItems(const QString &filename);

protected:
    QList<std::function<void()>> unloaders;

private:
    void unloadLocale();
};

#endif // QSLOCALEDIR_H
