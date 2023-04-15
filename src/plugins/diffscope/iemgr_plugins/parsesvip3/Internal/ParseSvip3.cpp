#include "ParseSvip3.h"

#include "Internal/Svip3Wizard.h"
#include "iemgr/IManager.h"

namespace IEMgr {

    namespace Internal {

        ParseSvip3::ParseSvip3() {
        }

        ParseSvip3::~ParseSvip3() {
        }

        bool ParseSvip3::initialize(const QStringList &arguments, QString *errorMessage) {
            IManager::instance()->addWizard(new Internal::Svip3Wizard());

            return true;
        }

        void ParseSvip3::extensionsInitialized() {
        }

        bool ParseSvip3::delayedInitialize() {
            return true;
        }

    }
}