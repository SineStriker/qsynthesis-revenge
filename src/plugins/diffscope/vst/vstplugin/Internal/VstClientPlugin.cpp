#include "VstClientPlugin.h"

#include <QApplication>
#include <QThread>

#include <cmath>

#include "AddOn/VstClientAddOn.h"
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

            generateVstConfig();

            auto srcNode = new QRemoteObjectHost(QUrl("local:" + GLOBAL_UUID), this);
            auto vstBridge = new VstBridge(this);
            srcNode->enableRemoting(vstBridge);

            auto actionMgr = ICore::instance()->actionSystem();

            // Add basic windows and add-ons

            auto winMgr = ICore::instance()->windowSystem();
            winMgr->addAddOn("project", &VstClientAddOn::staticMetaObject);

            return true;
        }

        void VstClientPlugin::generateVstConfig() {
            QFile configFile(QMFs::appDataPath() + "/ChorusKit/DiffScope/vstconfig.txt");
            configFile.open(QFile::WriteOnly | QFile::Text);
            if(configFile.isOpen()) {
                QTextStream stream(&configFile);
                stream << QDir::toNativeSeparators(QApplication::applicationDirPath()) << QDir::separator() << Qt::endl;
                stream << "vstbridge." <<
#ifdef Q_OS_WINDOWS
                    "dll"
#elif defined(Q_OS_MAC)
                    "dylib"
#else
                    "so"
#endif
                    << Qt::endl;
                stream << QDir::toNativeSeparators(QApplication::applicationFilePath()) << Qt::endl;
                stream.flush();
                configFile.close();
            } else {
                qDebug() << "VST Plugin: Cannot write vstconfig.txt " << configFile.errorString();
            }
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
