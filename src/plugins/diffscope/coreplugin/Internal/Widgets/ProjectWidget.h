#ifndef CHORUSKIT_PROJECTWIDGET_H
#define CHORUSKIT_PROJECTWIDGET_H

#include <QFrame>
#include <QLabel>
#include <QToolBar>
#include <QVBoxLayout>

#include <CDockFrame.h>
#include <Widgets/SynthVSplitter.h>

#include "Interfaces/IPianoKeyWidget.h"

namespace Core::Internal {

    class ProjectWidget : public QFrame {
        Q_OBJECT
    public:
        explicit ProjectWidget(QWidget *parent = nullptr);
        ~ProjectWidget();

        QToolBar *mainToolbar() const {
            return m_toolbar;
        }

        CDockFrame *mainDock() const {
            return m_frame;
        }

        QsApi::SynthVSplitter *canvas() const {
            return m_canvas;
        }

        QWidget *pianoKeyArea() const{
            return m_pianoArea;
        }

        Core::IPianoKeyWidget *pianoKeyWidget() const;
        void setPianoKeyWidget(Core::IPianoKeyWidget *widget);

    private:
        QVBoxLayout *m_layout;

        QToolBar *m_toolbar;
        CDockFrame *m_frame;
        QsApi::SynthVSplitter *m_canvas;

        QWidget *m_pianoArea;
        QWidget *m_sectionArea;

        QGridLayout *m_pianoLayout;
        QWidget *m_pianoWidget;
    };

}



#endif // CHORUSKIT_PROJECTWIDGET_H
