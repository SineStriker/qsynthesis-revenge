#ifndef IPROJECTWINDOW_P_H
#define IPROJECTWINDOW_P_H

#include <QToolBar>

#include <CoreApi/ActionContext.h>

#include "ICoreWindow_p.h"
#include "IProjectWindow.h"
#include "Internal/Widgets/FloatingTitleBar.h"
#include "PianoRoll/PianoRoll.h"

namespace Core {

    class IProjectWindowPrivate : public ICoreWindowPrivate {
        Q_OBJECT
        Q_DECLARE_PUBLIC(IProjectWindow)
    public:
        IProjectWindowPrivate();

        void init();

        void reloadStrings();

        void reloadMainToolbar();

        ActionContext *mainToolbarCtx;

        DspxDocument *m_doc;

        bool m_forceClose;

        // Layout
        QVBoxLayout *m_layout;
        QWidget *m_centralWidget;

        QToolBar *m_toolbar;
        CDockFrame *m_frame;

        PianoRoll *m_pianoRoll;

    private:
        void _q_documentChanged();
        void _q_documentRaiseRequested();
        void _q_documentCloseRequested();
    };

}


#endif // IPROJECTWINDOW_P_H
