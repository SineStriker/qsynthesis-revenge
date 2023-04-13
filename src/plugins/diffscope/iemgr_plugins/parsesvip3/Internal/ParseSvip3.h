#ifndef CHORUSKIT_PARSESVIP3_H
#define CHORUSKIT_PARSESVIP3_H

#include <extensionsystem/iplugin.h>

#include "parsesvip3/ParseSvip3Global.h"

namespace IEMgr {

    namespace Internal {

        class ParseSvip3 : public ExtensionSystem::IPlugin {
            Q_OBJECT
            Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
        public:
            ParseSvip3();
            ~ParseSvip3();

            bool initialize(const QStringList &arguments, QString *errorMessage) override;
            void extensionsInitialized() override;
            bool delayedInitialize() override;
        };

    } // namespace Internal

} // namespace Core


#endif // CHORUSKIT_PARSESVIP3_H
