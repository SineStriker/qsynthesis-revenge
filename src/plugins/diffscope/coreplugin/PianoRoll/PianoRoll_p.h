#ifndef PIANOROLLPRIVATE_H
#define PIANOROLLPRIVATE_H

#include <CDockFrame.h>

#include <CoreApi/ActionContext.h>

#include "PianoRoll.h"

#include "IPianoKeyWidget.h"
#include "Internal/Widgets/FloatingTitleBar.h"
#include "Internal/Widgets/PianoKeyContainer.h"
#include "Internal/Widgets/SectionWidget.h"

namespace Core {

    class PianoRollPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(PianoRoll)
    public:
        PianoRollPrivate();
        virtual ~PianoRollPrivate();

        void init();

        void readSettings();
        void saveSettings() const;

        PianoRoll *q_ptr;

        // Floating panels
        struct FloatingPanelControlBlock {
            QString key;
            QWidget *panel;
            Internal::FloatingTitleBar *titleBar;
            PianoRoll::FloatingPanelState state;
        };
        QHash<QString, FloatingPanelControlBlock> floatingPanels;

        // Piano key widgets
        QHash<QString, IPianoKeyWidgetFactory *> pianoKeyWidgets;
        QString currentPianoKeyWidget;

        void setPianoKeyWidget_helper(IPianoKeyWidget *w);

        // Layout
        QGridLayout *m_layout;

        QToolBar *m_toolbar;
        Internal::PianoKeyContainer *m_pianoKeyContainer;
        Internal::SectionWidget *m_sectionWidget;
        QsApi::SynthVSplitter *m_canvas;

        CanvasView *m_view;

        // Meta
        ActionContext *toolbarCtx;

        void adjustPianoKeyWidget();

        void reloadToolbar();

    private:
        void _q_viewMoved(const QPointF &pos, const QPointF &oldPos);
        void _q_viewResized(const QSizeF &size, const QSizeF &newSize);
        void _q_currentHeightChanged(int h);
    };

}

#endif // PIANOROLLPRIVATE_H