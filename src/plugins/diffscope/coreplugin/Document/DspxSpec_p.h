#ifndef CHORUSKIT_DSPXSPEC_P_H
#define CHORUSKIT_DSPXSPEC_P_H

#include <CoreApi/DocumentSpec_p.h>

#include "DspxSpec.h"

namespace Core {

    class DspxSpecPrivate : public QObject, public DocumentSpecPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(DspxSpec)
    public:
        DspxSpecPrivate();
        ~DspxSpecPrivate();

        void init();
    };

}

#endif // CHORUSKIT_DSPXSPEC_P_H
