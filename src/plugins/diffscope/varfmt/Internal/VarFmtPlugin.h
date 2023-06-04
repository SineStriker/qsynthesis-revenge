#ifndef VARFMTPLUGIN_H
#define VARFMTPLUGIN_H

#include <extensionsystem/iplugin.h>

namespace VarFmt {

    namespace Internal {

        class VarFmtPlugin : public ExtensionSystem::IPlugin {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
        public:
            VarFmtPlugin();
            ~VarFmtPlugin();

            bool initialize(const QStringList &arguments, QString *errorMessage) override;
            void extensionsInitialized() override;
            bool delayedInitialize() override;
        };

    } // namespace Internal

} // namespace Core


#endif // VARFMTPLUGIN_H
