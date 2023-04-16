#ifndef BATPROCPLUGIN_H
#define BATPROCPLUGIN_H

#include <extensionsystem/iplugin.h>

#include "ScriptMgrGlobal.h"

namespace ScriptMgr::Internal {

    class TestObject: public QObject {
        Q_OBJECT
    public slots:
        QString f(const QStringList &list) {
            qDebug() << list[0].toStdString().c_str();
            return list[1];
        }
    };

    class BatchProcess : public ExtensionSystem::IPlugin {
        Q_OBJECT
        Q_PLUGIN_METADATA(IID "org.ChorusKit.DiffScope.Plugin" FILE "plugin.json")
    public:
        BatchProcess();
        ~BatchProcess();

        bool initialize(const QStringList &arguments, QString *errorMessage) override;
        void extensionsInitialized() override;
        bool delayedInitialize() override;
    };

} // namespace Core


#endif // BATPROCPLUGIN_H
