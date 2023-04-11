#ifndef IWindowContextPRIVATE_H
#define IWindowContextPRIVATE_H

#include "IWindowContext.h"

namespace Core {

    class CKAPPCORE_API IWindowContextPrivate {
        Q_DECLARE_PUBLIC(IWindowContext)
    public:
        IWindowContextPrivate();
        virtual ~IWindowContextPrivate();

        void init();

        IWindowContext *q_ptr;

        Core::IWindow *iWin;
        QDateTime dt;
    };

}

#endif // IWindowContextPRIVATE_H