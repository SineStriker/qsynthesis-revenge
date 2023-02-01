#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QColor>
#include <QMap>
#include <QSet>

// Parse theme json file

class ThemeConfig {
public:
    ThemeConfig();
    ~ThemeConfig();

public:
    // Variables -> namespace - values
    QMap<QString, QMap<QString, QString>> colors;
    QMap<QString, QMap<QString, int>> sizes;

    QSet<QString> namespaces;

    int priority; // The lower the prior

    bool load(const QStringList &filenames);

protected:
    bool loadOne(const QString &filename);
};

#endif // THEMECONFIG_H
