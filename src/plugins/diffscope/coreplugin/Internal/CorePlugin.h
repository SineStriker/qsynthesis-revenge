#ifndef COREPLUGIN_H
#define COREPLUGIN_H

#include <extensionsystem/iplugin.h>

#include "CoreGlobal.h"

namespace Core {

    namespace Internal {

        class CORE_EXPORT CorePlugin : public ExtensionSystem::IPlugin {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
        public:
            CorePlugin();
            ~CorePlugin();

            bool initialize(const QStringList &arguments, QString *errorMessage) override;
            void extensionsInitialized() override;
            bool delayedInitialize() override;

            QObject *remoteCommand(const QStringList &options, const QString &workingDirectory,
                                   const QStringList &args) override;
        };

    } // namespace Internal

} // namespace Core

#endif // COREPLUGIN_H
