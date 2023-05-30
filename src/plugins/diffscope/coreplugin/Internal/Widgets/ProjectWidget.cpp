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

        m_piano = new QsApi::SynthVSplitter();
        m_piano->setObjectName("piano-roll");

        m_frame->setWidget(m_piano);
    }

    ProjectWidget::~ProjectWidget() {
    }

}