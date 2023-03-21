#ifndef PLUGINMETAJSON_H
#define PLUGINMETAJSON_H

#include <QStringList>


class PluginMetaJson {
public:
    PluginMetaJson();
    explicit PluginMetaJson(const QString &dir);
    ~PluginMetaJson();

    bool load();
    bool exists() const;

    QString dir;

    QString name;
    QStringList subdirs;
    bool allFiles;
    bool allSubdirs;

    QString pluginPath() const;
};

#endif // PLUGINMETAJSON_H
