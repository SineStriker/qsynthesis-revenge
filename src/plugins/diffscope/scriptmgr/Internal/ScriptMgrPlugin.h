#ifndef BATPROCPLUGIN_H
#define BATPROCPLUGIN_H

#include <extensionsystem/iplugin.h>

#include "ScriptMgrGlobal.h"

namespace ScriptMgr::Internal {

    class ScriptMgrPlugin : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
    public:
        ScriptMgrPlugin();
        ~ScriptMgrPlugin();

        bool initialize(const QStringList &arguments, QString *errorMessage) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

} // namespace Core


#endif // BATPROCPLUGIN_H
