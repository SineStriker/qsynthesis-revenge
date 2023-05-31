#include "ProjectWidget.h"

#include <QLabel>
#include <QVBoxLayout>

#include <CToolBar.h>

namespace Core::Internal {

    ProjectWidget::ProjectWidget(QWidget *parent) : QFrame(parent) {
        m_toolbar = new CToolBar();
        m_toolbar->setObjectName("main-toolbar");
        m_toolbar->setMovable(false);
        m_toolbar->setFloatable(false);
        m_toolbar->setOrientation(Qt::Horizontal);
        m_toolbar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

        m_frame = new CDockFrame();
        m_frame->setObjectName("main-dock");

        m_layout = new QVBoxLayout();
        m_layout->setMargin(0);
        m_layout->setSpacing(0);
        m_layout->addWidget(m_toolbar);
        m_layout->addWidget(m_frame, 1);
        setLayout(m_layout);

        m_canvas = new QsApi::SynthVSplitter();
        m_canvas->setObjectName("canvas");

        m_pianoKeyContainer = new PianoKeyContainer();
        m_pianoKeyContainer->setObjectName("piano-container");

        m_sectionContainer = new QWidget();
        m_sectionContainer->setObjectName("section-container");

        m_pianoRollLayout = new QGridLayout();
        m_pianoRollLayout->setMargin(0);
        m_pianoRollLayout->setSpacing(0);
        m_pianoRollLayout->addItem(new QSpacerItem(0, 0), 0, 0);
        m_pianoRollLayout->addWidget(m_sectionContainer, 0, 1);
        m_pianoRollLayout->addWidget(m_pianoKeyContainer, 1, 0);
        m_pianoRollLayout->addWidget(m_canvas, 1, 1);

        m_pianoRoll = new QWidget();
        m_pianoRoll->setAttribute(Qt::WA_StyledBackground);
        m_pianoRoll->setLayout(m_pianoRollLayout);

        m_frame->setWidget(m_pianoRoll);
    }

    ProjectWidget::~ProjectWidget() {
    }

    Core::IPianoKeyWidget *ProjectWidget::pianoKeyWidget() const {
        return m_pianoKeyContainer->area();
    }

    void ProjectWidget::setPianoKeyWidget(IPianoKeyWidget *widget) {
        m_pianoKeyContainer->setArea(widget);
    }

}