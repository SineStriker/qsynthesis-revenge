#ifndef IMANAGERPRIVATE_H
#define IMANAGERPRIVATE_H

#include "IManager.h"

#include <Collections/QMChronMap.h>

namespace IEMgr {

    class IManagerPrivate {
        Q_DECLARE_PUBLIC(IManager)
    public:
        IManagerPrivate();
        virtual ~IManagerPrivate();

        void init();

        IManager *q_ptr;

        bool running;
        QMChronMap<QString, IImportWizard *> importWizards;
    };

}

#endif // IMANAGERPRIVATE_H