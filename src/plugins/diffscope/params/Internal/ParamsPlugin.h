#ifndef CHORUSKIT_PARAMSPLUGIN_H
#define CHORUSKIT_PARAMSPLUGIN_H

#include <extensionsystem/iplugin.h>

#include "ParamsGlobal.h"

namespace Params {

    namespace Internal {

        class ParamsPlugin : public ExtensionSystem::IPlugin {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
        public:
            ParamsPlugin();
            ~ParamsPlugin();

            bool initialize(const QStringList &arguments, QString *errorMessage) override;
            void extensionsInitialized() override;
            bool delayedInitialize() override;
        };

    } // namespace Internal

} // namespace Core



#endif // CHORUSKIT_PARAMSPLUGIN_H
