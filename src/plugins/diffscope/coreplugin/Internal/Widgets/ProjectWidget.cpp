#include "ProjectWidget.h"

#include <QLabel>
#include <QVBoxLayout>

namespace Core::Internal {

    ProjectWidget::ProjectWidget(QWidget *parent) : QWidget(parent) {
        auto layout = new QVBoxLayout();
        auto label = new QLabel("234253235");
        layout->addWidget(label);
        setLayout(layout);
    }

    ProjectWidget::~ProjectWidget() {
    }

}