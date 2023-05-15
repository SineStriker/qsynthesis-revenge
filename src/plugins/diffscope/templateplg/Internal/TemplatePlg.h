#ifndef TEMPLATEPLG_H
#define TEMPLATEPLG_H

#include <QObject>
#include <extensionsystem/iplugin.h>

namespace TemplatePlg {

    class TmplPlg : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
    public:
        TmplPlg();
        ~TmplPlg();

        bool initialize(const QStringList &arguments, QString *errorMessage) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

}

#endif // TEMPLATEPLG_H