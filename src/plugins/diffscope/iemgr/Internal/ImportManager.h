#ifndef IMPORTMANAGER_H
#define IMPORTMANAGER_H

#include <extensionsystem/iplugin.h>

#include "IEMgrGlobal.h"

namespace IEMgr {

    namespace Internal {

        class ImportManager : public ExtensionSystem::IPlugin {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
        public:
            ImportManager();
            ~ImportManager();

            bool initialize(const QStringList &arguments, QString *errorMessage) override;
            void extensionsInitialized() override;
            bool delayedInitialize() override;
        };

    } // namespace Internal

} // namespace Core


#endif // IMPORTMANAGER_H
