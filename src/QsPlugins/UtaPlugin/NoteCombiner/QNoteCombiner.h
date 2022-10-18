#ifndef QNOTECOMBINER_H
#define QNOTECOMBINER_H

#include "api/IUtaPlugin.h"

#include <QObject>

class Q_DECL_EXPORT QNoteCombiner : public QObject, public IUtaPlugin {
    Q_OBJECT
    Q_INTERFACES(IUtaPlugin)
    Q_PLUGIN_METADATA(IID IUtaPlugin_IID FILE "plugin.json")
public:
    explicit QNoteCombiner(QObject *parent = nullptr);
    ~QNoteCombiner();

    int exec(PluginArguments *args) override;
};

#endif // QNOTECOMBINER_H
