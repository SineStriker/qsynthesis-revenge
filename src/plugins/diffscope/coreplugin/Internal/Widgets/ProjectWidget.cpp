#include "ProjectWidget.h"

#include <QLabel>
#include <QVBoxLayout>

#include <CToolBar.h>

#include "TPianoArea/TPianoArea.h"
#include "TPianoArea/TPianoScroll.h"

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

        auto pianoScroll = new TPianoScroll();
        m_pianoArea = pianoScroll;

        auto sectionArea = new QWidget();
        m_sectionArea = sectionArea;

        m_pianoLayout = new QGridLayout();
        m_pianoLayout->setMargin(0);
        m_pianoLayout->setSpacing(0);
        m_pianoLayout->addItem(new QSpacerItem(0, 0), 0, 0);
        m_pianoLayout->addWidget(m_sectionArea, 0, 1);
        m_pianoLayout->addWidget(m_pianoArea, 1, 0);
        m_pianoLayout->addWidget(m_canvas, 1, 1);

        m_pianoWidget = new QWidget();
        m_pianoWidget->setAttribute(Qt::WA_StyledBackground);
        m_pianoWidget->setLayout(m_pianoLayout);

        m_frame->setWidget(m_pianoWidget);
    }

    ProjectWidget::~ProjectWidget() {
    }

    Core::IPianoKeyWidget *ProjectWidget::pianoKeyWidget() const {
        auto pianoScroll = qobject_cast<TPianoScroll *>(m_pianoArea);
        return pianoScroll->area();
    }

    void ProjectWidget::setPianoKeyWidget(Core::IPianoKeyWidget *widget) {
        auto pianoScroll = qobject_cast<TPianoScroll *>(m_pianoArea);
        pianoScroll->setArea(widget);
    }

}