#ifndef IWIZARDFACTORYPRIVATE_H
#define IWIZARDFACTORYPRIVATE_H

#include "IWizardFactory.h"

namespace IEMgr {

    class IWizardFactoryPrivate {
        Q_DECLARE_PUBLIC(IWizardFactory)
    public:
        IWizardFactoryPrivate();
        virtual ~IWizardFactoryPrivate();

        void init();

        IWizardFactory *q_ptr;

        QString id;

        QString displayName;
        QString description;

        QString iconPath;

        QString category;
        QString displayCategory;
    };

}

#endif // IWIZARDFACTORYPRIVATE_H