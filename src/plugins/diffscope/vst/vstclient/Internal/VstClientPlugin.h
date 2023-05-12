#ifndef VSTCLIENTPLUGIN_H
#define VSTCLIENTPLUGIN_H

#include <extensionsystem/iplugin.h>

#include "VstClientGlobal.h"

namespace Vst::Internal {

    class VstClientPlugin : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
    public:
        VstClientPlugin();
        ~VstClientPlugin();

        bool initialize(const QStringList &arguments, QString *errorMessage) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

} // namespace Core


#endif // VSTCLIENTPLUGIN_H
