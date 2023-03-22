#ifndef LOADCONFIG_H
#define LOADCONFIG_H

#include <QStringList>

#include "qjsonstream.h"

struct SplashText {
    QAS_JSON(SplashText)

    QList<int> anchor;
    bool rightToLeft;
    int fontSize;

    SplashText();
};

struct SplashSettings {
    QAS_JSON(SplashSettings)

    QList<int> splashSize;
    SplashText copyrightText;
    SplashText loaderText;
};

struct LoadConfig {
    QAS_JSON(LoadConfig)

    QString pluginIID;
    QString splashImage;
    SplashSettings splashSettings;
    QString coreName;
    QStringList resourceFiles;

    bool load(const QString &filename);
};


#endif // LOADCONFIG_H
