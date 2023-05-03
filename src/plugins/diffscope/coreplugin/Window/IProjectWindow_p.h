#ifndef IPROJECTWINDOW_P_H
#define IPROJECTWINDOW_P_H

#include "ICoreWindow_p.h"
#include "IProjectWindow.h"

#include <CoreApi/ActionContext.h>

namespace Core {

    class IProjectWindowPrivate : public ICoreWindowPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(IProjectWindow)
    public:
        IProjectWindowPrivate();

        void init();

        void reloadStrings();

        ActionContext *mainMenuCtx;

        DspxDocument *m_doc;

        bool m_forceClose;

    private:
        void _q_documentChanged();
        void _q_documentRaiseRequested();
        void _q_documentCloseRequested();
    };

}


#endif // IPROJECTWINDOW_P_H
