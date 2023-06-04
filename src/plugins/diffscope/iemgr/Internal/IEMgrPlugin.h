#ifndef IMPORTEXPORTMANAGER_H
#define IMPORTEXPORTMANAGER_H

#include <extensionsystem/iplugin.h>

namespace IEMgr {

    namespace Internal {

        class IEMgrPlugin : public ExtensionSystem::IPlugin {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
        public:
            IEMgrPlugin();
            ~IEMgrPlugin();

            bool initialize(const QStringList &arguments, QString *errorMessage) override;
            void extensionsInitialized() override;
            bool delayedInitialize() override;
        };

    } // namespace Internal

} // namespace Core


#endif // IMPORTEXPORTMANAGER_H
