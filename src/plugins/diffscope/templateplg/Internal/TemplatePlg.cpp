#include "TemplatePlg.h"

#include "CoreApi/ICoreBase.h"
#include "CoreApi/ILoader.h"
#include "CoreApi/SettingCatalog.h"
#include "Settings/ConfigPage.h"
#include "Settings/JsonConfigPage.h"
#include <QApplication>
#include <QMDecoratorV2.h>
#include <QMSystem.h>
#include <QThread>
#include <extensionsystem/pluginspec.h>


namespace TemplatePlg {

    using namespace Core;

    TmplPlg::TmplPlg() {
    }

    TmplPlg::~TmplPlg() {
    }

    // Plugin Initialize
    bool TmplPlg::initialize(const QStringList &arguments, QString *errorMessage) {
        // Add resources
        qIDec->addTranslationPath(pluginSpec()->location() + "/translations");
        // qIDec->addThemePath(pluginSpec()->location() +"/themes");

        // Add initialization information
        auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
        if (splash) {
            splash->showMessage(tr("Initializing Template Plugin..."));
        }
        // QThread::msleep(2000);

        // Add Setting Page
        auto configPage = new ConfigPage();
        ICoreBase::instance()->settingCatalog()->addPage(configPage);

        auto jsonConfigPage = new JsonConfigPage();
        ICoreBase::instance()->settingCatalog()->addPage(jsonConfigPage);

        return true;
    }

    void TmplPlg::extensionsInitialized() {
    }

    bool TmplPlg::delayedInitialize() {
        return false;
    }



}