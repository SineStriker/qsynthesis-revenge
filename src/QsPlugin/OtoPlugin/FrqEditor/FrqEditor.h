#ifndef FRQEDITOR_H
#define FRQEDITOR_H

#include "Api/IOtoPlugin.h"

class Q_DECL_EXPORT FrqEditor : public IOtoPlugin {
    Q_OBJECT
    Q_INTERFACES(IOtoPlugin)
    Q_PLUGIN_METADATA(IID IOtoPlugin_IID FILE "plugin.json")
public:
    explicit FrqEditor(QObject *parent = nullptr);
    ~FrqEditor();

    int exec(const PluginArguments &args, void *userdata) override;

signals:
};

#endif // FRQEDITOR_H
