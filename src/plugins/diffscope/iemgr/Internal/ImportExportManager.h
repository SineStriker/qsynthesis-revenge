#ifndef IMPORTEXPORTMANAGER_H
#define IMPORTEXPORTMANAGER_H

#include <extensionsystem/iplugin.h>

#include "IEMgrGlobal.h"

namespace IEMgr {

    namespace Internal {

        class ImportExportManager : public ExtensionSystem::IPlugin {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
        public:
            ImportExportManager();
            ~ImportExportManager();

            bool initialize(const QStringList &arguments, QString *errorMessage) override;
            void extensionsInitialized() override;
            bool delayedInitialize() override;
        };

    } // namespace Internal

} // namespace Core


#endif // IMPORTEXPORTMANAGER_H
