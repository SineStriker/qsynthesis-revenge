#include "ParamsPlugin.h"

#include <QSplashScreen>

#include <extensionsystem/pluginspec.h>

#include <QMDecoratorV2.h>

#include <CoreApi/ILoader.h>

#include <coreplugin/ICore.h>

#include <Internal/AddOn/ParamsAddOn.h>

namespace Params::Internal {

    using namespace Core;

    ParamsPlugin::ParamsPlugin() {
    }

    ParamsPlugin::~ParamsPlugin() {
    }

    bool ParamsPlugin::initialize(const QStringList &arguments, QString *errorMessage) {
        // Add resources
        qIDec->addTranslationPath(pluginSpec()->location() + "/translations");
        qIDec->addThemePath(pluginSpec()->location() + "/themes");

        auto splash = qobject_cast<QSplashScreen *>(
            ILoader::instance()->getFirstObject("choruskit_init_splash"));
        if (splash) {
            splash->showMessage(tr("Initializing param-curve editor..."));
        }
        // QThread::msleep(2000);

        auto icore = ICore::instance();

        // Add basic actions
        auto actionMgr = icore->actionSystem();

        if (actionMgr->loadContexts(":/params_actions.xml").isEmpty()) {
            *errorMessage = tr("Failed to load action configuration!");
            return false;
        }

        // Add basic windows and add-ons
        auto winMgr = icore->windowSystem();
        winMgr->addAddOn("project", &Internal::ParamsAddOn::staticMetaObject);
        return true;
    }

    void ParamsPlugin::extensionsInitialized() {
    }

    bool ParamsPlugin::delayedInitialize() {
        return false;
    }

} // namespace Params::Internal
