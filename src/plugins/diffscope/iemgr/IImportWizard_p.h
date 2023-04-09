#ifndef IIMPORTWIZARDPRIVATE_H
#define IIMPORTWIZARDPRIVATE_H

#include "IImportWizard.h"

namespace IEMgr {

    class WizardContextPrivate{
    public:
        QDateTime time;
        Core::IWindow *winHandle;
    };

    class IImportWizardPrivate {
        Q_DECLARE_PUBLIC(IImportWizard)
    public:
        IImportWizardPrivate();
        virtual ~IImportWizardPrivate();

        void init();

        IImportWizard *q_ptr;

        QString id;

        QString displayName;
        QString description;

        QString iconPath;

        QString category;
        QString displayCategory;
    };

}

#endif // IIMPORTWIZARDPRIVATE_H