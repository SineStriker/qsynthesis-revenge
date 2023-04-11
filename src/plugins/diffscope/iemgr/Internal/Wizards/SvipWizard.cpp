#include "SvipWizard.h"

#include <QNrbfStream.h>

namespace IEMgr ::Internal {

    SvipWizard::SvipWizard(QObject *parent) : IWizardFactory("iemgr.SvipWizard", parent) {
        setDisplayName(tr("SVIP file"));
        setDescription(tr("XStudio 2.0 project file."));

        setCategory("score.XiaoIce");
        setDisplayCategory(tr("XiaoIce"));
    }

    SvipWizard::~SvipWizard() {
    }

    QString SvipWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QString("%1(%2)").arg(tr("XStudio Svip Files"), "*.svip") + ";;" + IWizardFactory::filter(feature);
    }

    IWizardFactory::Features SvipWizard::features() const {
        return {ImportProject | ExportProject};
    }

    bool SvipWizard::runWizard(Feature feature, IWizardContext *context) {
        return false;
    }

}