#ifndef LOADCONFIG_H
#define LOADCONFIG_H

#include <QStringList>

class LoadConfig {
public:
    QString appName;
    QString appVersion;
    QString appOrganizationName;
    QString appOrganizationDomain;

    QString pluginIID;
    QString splashImage;
    QString coreName;

    QStringList resourceFiles;

    bool load(const QString &filename);
};


#endif // LOADCONFIG_H
