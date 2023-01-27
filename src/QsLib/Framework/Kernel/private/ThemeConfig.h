#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QColor>
#include <QMap>

// Parse theme json file

class ThemeConfig {
public:
    ThemeConfig();
    ~ThemeConfig();

public:
    QMap<QString, QColor> colors;
    QMap<QString, int> sizes;

    int priority;           // The lower the prior
    QStringList namespaces; // Namespaces from config

    bool load(const QString &filename, bool configOnly);
};

#endif // THEMECONFIG_H
