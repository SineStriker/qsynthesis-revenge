#include "VstClientPlugin.h"

#include <QApplication>
#include <QThread>

#include "CoreApi/ILoader.h"
#include "IpcClient.h"
#include "IpcReceiveChannel.h"

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>


namespace Vst {

    using namespace Core;

    namespace Internal {

        VstClientPlugin::VstClientPlugin() {
        }

        VstClientPlugin::~VstClientPlugin() {
        }


        bool VstClientPlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(pluginSpec()->location() + "/translations");

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing vst support..."));
            }

            auto *ipcClient = new IpcClient("77F6E993-671E-4283-99BE-C1CD1FF5C09E", this);
            auto *ipcReceiveChannel = new IpcReceiveChannel(ipcClient);
            connect(ipcReceiveChannel, &IpcReceiveChannel::received, this,
                    [=](quint8 signal, const QByteArray &payload, QByteArray &ret) { qDebug() << payload; });

            auto actionMgr = ICore::instance()->actionSystem();

            // Add basic windows and add-ons

            return true;
        }

        void VstClientPlugin::extensionsInitialized() {
        }

        bool VstClientPlugin::delayedInitialize() {
            return false;
        }

        QObject *VstClientPlugin::remoteCommand(const QStringList &options, const QString &workingDirectory,
                                                const QStringList &args) {
            return nullptr;
        }

    }

}