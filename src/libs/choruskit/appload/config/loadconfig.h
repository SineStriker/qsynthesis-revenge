#ifndef LOADCONFIG_H
#define LOADCONFIG_H

#include <QStringList>

#include "qjsonstream.h"

struct SplashText {
    QAS_JSON(SplashText)

    QList<int> pos;
    QList<int> anchor;
    int fontSize;
    QString fontColor;
    int maxWidth;

    QString text;

    SplashText();
};

struct SplashSettings {
    QAS_JSON(SplashSettings)

    QList<int> size;
    QMap<QString, SplashText> texts;
};

struct LoadConfig {
    QAS_JSON(LoadConfig)

    QString pluginIID;
    QString splashImage;
    SplashSettings splashSettings;
    QString coreName;
    QStringList resourceFiles;
    bool resizable;
    QList<int> splashSize;

    bool load(const QString &filename);
};


#endif // LOADCONFIG_H
