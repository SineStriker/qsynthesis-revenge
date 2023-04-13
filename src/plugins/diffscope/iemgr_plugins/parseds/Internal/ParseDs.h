#ifndef PARSEDS_H
#define PARSEDS_H

#include <extensionsystem/iplugin.h>

namespace IEMgr {

    namespace Internal {

        class ParseDs : public ExtensionSystem::IPlugin {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
        public:
            ParseDs();
            ~ParseDs();

            bool initialize(const QStringList &arguments, QString *errorMessage) override;
            void extensionsInitialized() override;
            bool delayedInitialize() override;
        };

    } // namespace Internal

} // namespace Core


#endif // PARSEDS_H
