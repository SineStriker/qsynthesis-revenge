#include "HomeProjConfWidget.h"

#include <QDir>

#include "Kernel/LvApplication.h"

HomeProjConfWidget::HomeProjConfWidget(QWidget *parent) : QFrame(parent) {
    titleLabel = new QLabel();
    titleLabel->setObjectName("title-label");

    subtitleLabel = new QLabel();
    subtitleLabel->setObjectName("subtitle-label");
    subtitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    projectName = new LabelTextBlock(false);
    projectName->setObjectName("project-name-block");

    location = new LabelTextBlock(true);
    projectName->setObjectName("location-block");

    solution = new LabelTextBlock(false);
    solution->setObjectName("solution-block");

    backButton = new CPushButton();
    backButton->setObjectName("prev-button");

    createButton = new CPushButton();
    createButton->setObjectName("ok-button");

    buttonLayout = new QHBoxLayout();
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(0);

    buttonLayout->addStretch();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(createButton);

    // Entities
    entityLayout = new QVBoxLayout();
    entityLayout->setMargin(0);
    entityLayout->setSpacing(0);

    entityLayout->addWidget(titleLabel);
    entityLayout->addWidget(subtitleLabel);

    entityLayout->addWidget(projectName);
    entityLayout->addWidget(location);
    entityLayout->addWidget(solution);

    upperLayout = new QHBoxLayout();
    upperLayout->setMargin(0);
    upperLayout->setSpacing(0);

    upperLayout->addLayout(entityLayout);
    upperLayout->addStretch();

    upperLayout->setStretch(0, 3);
    upperLayout->setStretch(1, 1);

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addLayout(upperLayout);
    mainLayout->addStretch();
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    connect(backButton, &QPushButton::clicked, this, &HomeProjConfWidget::_q_backButtonClicked);
    connect(createButton, &QPushButton::clicked, this, &HomeProjConfWidget::_q_createButtonClicked);
}

HomeProjConfWidget::~HomeProjConfWidget() {
}

void HomeProjConfWidget::reloadStrings() {
    titleLabel->setText(tr("Configure New Project"));
    subtitleLabel->setText(tr("Empty Template"));

    projectName->label->setText(tr("Project Name"));
    location->label->setText(tr("Location"));
    location->browseButton->setText(tr("..."));
    solution->label->setText(tr("Definition File Name"));

    backButton->setText(tr("Back"));
    createButton->setText(tr("Create"));
}

void HomeProjConfWidget::clear() {
    projectName->text->setText(tr("LVProject1"));
    location->text->setText(QDir::toNativeSeparators(qApp->desktopDir()));
    solution->text->setText(tr("LVProject1"));
}

QTypeList HomeProjConfWidget::styleData() const {
    return {};
}

void HomeProjConfWidget::setStyleData(const QTypeList &list) {
}

void HomeProjConfWidget::_q_backButtonClicked() {
    emit canceled();
}

void HomeProjConfWidget::_q_createButtonClicked() {
    emit confirmed();
}
