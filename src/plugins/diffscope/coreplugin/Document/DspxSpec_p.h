#ifndef CHORUSKIT_DSPXSPEC_P_H
#define CHORUSKIT_DSPXSPEC_P_H

#include "DspxSpec.h"

namespace Core {

    class DspxSpecPrivate : public QObject {
        Q_OBJECT
    public:
        DspxSpecPrivate(DspxSpec *q);
        ~DspxSpecPrivate();

        void init();

        DspxSpec *q;

        mutable QsApi::AceTreeSerializer *serializer;

        void reloadStrings();
    };

}

#endif // CHORUSKIT_DSPXSPEC_P_H
