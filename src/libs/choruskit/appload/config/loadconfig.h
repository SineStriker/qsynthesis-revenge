#ifndef LOADCONFIG_H
#define LOADCONFIG_H

#include <QStringList>

struct SplashSettings{
};

class LoadConfig {
public:
    QString pluginIID;
    QString splashImage;
    QString coreName;

    QStringList resourceFiles;

    bool load(const QString &filename);
};


#endif // LOADCONFIG_H
