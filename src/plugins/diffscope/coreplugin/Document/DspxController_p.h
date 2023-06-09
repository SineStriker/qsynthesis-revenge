#ifndef CHORUSKIT_DSPXCONTROLLER_P_H
#define CHORUSKIT_DSPXCONTROLLER_P_H

#include <Controllers/AceTreeController_p.h>

#include "DspxController.h"

namespace Core {

    class DspxRootControllerPrivate : public QsApi::AceTreeControllerPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(DspxRootController)
    public:
        DspxRootControllerPrivate();
        ~DspxRootControllerPrivate();

        void init();

        QsApi::AceTreeItem *metadata, *workspace;

    private:
        void _q_propertyChanged(QsApi::AceTreeItem *item, const QString &key, const QVariant &oldValue,
                                const QVariant &newValue);
    };
}

#endif // CHORUSKIT_DSPXCONTROLLER_P_H
