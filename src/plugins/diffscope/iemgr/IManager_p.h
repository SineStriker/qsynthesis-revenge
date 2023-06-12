#ifndef IMANAGERPRIVATE_H
#define IMANAGERPRIVATE_H

#include "IManager.h"

#include <QMChronMap.h>

namespace IEMgr {

    class IManagerPrivate {
        Q_DECLARE_PUBLIC(IManager)
    public:
        IManagerPrivate();
        virtual ~IManagerPrivate();

        void init();

        IManager *q_ptr;

        bool running;
        QMChronMap<QString, IWizardFactory *> importWizards;
    };

}

#endif // IMANAGERPRIVATE_H