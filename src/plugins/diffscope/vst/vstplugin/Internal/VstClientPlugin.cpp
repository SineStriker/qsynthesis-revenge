#include "VstClientPlugin.h"

#include <QApplication>
#include <QThread>

#include <cmath>

#include "CoreApi/ILoader.h"

#include "AddOn/VstClientAddOn.h"
#include "Connection/VstBridge.h"
#include "Settings/VstSettingPage.h"
#include "VstHelper.h"

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

            new VstHelper(this);
            auto rout =
                qobject_cast<Core::InitialRoutine *>(ILoader::instance()->getFirstObject("choruskit_init_routine"));
            if (rout) {
                rout->splash()->showMessage(tr("Initializing VST plugin..."));
                connect(rout, &Core::InitialRoutine::done, this, [=]{
                    VstHelper::instance()->isLoadFromInitialization = false;
                });
            }

            VstHelper::generateVstConfig();

            VstHelper::instance()->isLoadFromInitialization = arguments.contains("-vst");


            auto actionMgr = ICore::instance()->actionSystem();

            // Add basic windows and add-ons

            auto winMgr = ICore::instance()->windowSystem();
            winMgr->addAddOn("project", &VstClientAddOn::staticMetaObject);

            ICore::instance()->settingCatalog()->addPage(new VstSettingPage);

            if(!VstHelper::instance()->startRemoting()) {
                qWarning() << "VstClientPlugin: Cannot start remoting.";
            }

            return true;
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
                if(VstHelper::instance()->vstBridge) VstHelper::instance()->vstBridge->showWindow();
            } else {
                qDebug() << "VstClientPlugin: Remote command from standalone mode";
                // if there exists vst window and vst window is the only window, then show home window.
                if(VstHelper::instance()->addOn && Core::ICore::instance()->windowSystem()->count() == 1) {
                    Core::ICore::showHome();
                }
            }
            return nullptr;
        }

    }

}
