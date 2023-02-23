#ifndef THEMECONFIG_H
#define THEMECONFIG_H

#include <QColor>
#include <QMap>
#include <QSet>

#include <QJsonValue>

// Parse theme json file

class ThemeConfig {
public:
    ThemeConfig();
    ~ThemeConfig();

public:
    struct Value {
        double ratio;
        QJsonValue val;
    };

    // Variables -> namespace - values
    QMap<QString, QMap<QString, Value>> values;
    QSet<QString> namespaces;

    int priority; // The lower the prior
    double ratio;

    bool load(const QStringList &filenames);

protected:
    bool loadOne(const QString &filename);
};

#endif // THEMECONFIG_H
