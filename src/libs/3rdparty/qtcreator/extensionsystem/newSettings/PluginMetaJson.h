#ifndef PLUGINMETAJSON_H
#define PLUGINMETAJSON_H

#include <QStringList>


class PluginMetaJson {
public:
    PluginMetaJson();
    explicit PluginMetaJson(const QString &dir);
    ~PluginMetaJson();

    bool load();
    bool save() const;

    QString dir;
    QString name;

    QString pluginPath() const;
};

#endif // PLUGINMETAJSON_H
