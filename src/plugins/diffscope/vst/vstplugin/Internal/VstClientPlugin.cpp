#include "VstClientPlugin.h"

#include <QApplication>
#include <QThread>

#include <cmath>

#include "CoreApi/ILoader.h"

#include "AddOn/VstClientAddOn.h"
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

            auto rout =
                qobject_cast<Core::InitialRoutine *>(ILoader::instance()->getFirstObject("choruskit_init_routine"));
            if (rout) {
                rout->splash()->showMessage(tr("Initializing VST plugin..."));
                connect(rout, &Core::InitialRoutine::done, this, [=]{
                    vstBridge->m_isLoadFromInitialization = false;
                });
            }

            generateVstConfig();

            auto srcNode = new QRemoteObjectHost(QUrl("local:" + GLOBAL_UUID), this);
            vstBridge = new VstBridge(this);
            vstBridge->m_isLoadFromInitialization = arguments.contains("-vst");
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
            if (configFile.isOpen()) {
                QTextStream stream(&configFile);
                stream << QDir::toNativeSeparators(QApplication::applicationDirPath()) << QDir::separator() << Qt::endl;
                stream << "vstbridge."
                       <<
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
            if(options.contains("-vst")) {
                qDebug() << "VstClientPlugin: Remote command from vst mode";
                if(vstBridge) vstBridge->showWindow();
            } else {
                qDebug() << "VstClientPlugin: Remote command from standalone mode";
                // if there exists vst window and vst window is the only window, then show home window.
                if(vstBridge->m_clientAddOn && Core::ICore::instance()->windowSystem()->count() == 1) {
                    Core::ICore::showHome();
                }
            }
            return nullptr;
        }

    }

}
