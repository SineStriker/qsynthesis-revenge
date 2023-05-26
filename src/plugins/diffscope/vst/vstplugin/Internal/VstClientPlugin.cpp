#include "VstClientPlugin.h"

#include <QApplication>
#include <QThread>

#include <cmath>

#include "CoreApi/ILoader.h"
#include "VstBridge.h"

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <QMessageBox>
#include <QtRemoteObjects>
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

            auto srcNode = new QRemoteObjectHost(QUrl("local:77F6E993-671E-4283-99BE-C1CD1FF5CO9E"), this);
            auto vstBridge = new VstBridge(this);
            srcNode->enableRemoting(vstBridge);


            if(arguments.contains("-vst")) {

            }

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