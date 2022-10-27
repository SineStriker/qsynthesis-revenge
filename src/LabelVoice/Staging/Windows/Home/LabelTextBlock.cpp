#include "LabelTextBlock.h"

#include <QDir>

#include "Managers/FileManager.h"

const char FLAG_BROWSE[] = "%BROWSE%";

LabelTextBlock::LabelTextBlock(bool browse, QWidget *parent) : QWidget(parent) {
    label = new QLabel();
    label->setObjectName("label");

    text = new QLineEdit();
    text->setObjectName("text");

    if (browse) {
        browseButton = new CPushButton();
        browseButton->setObjectName("browse-button");
        connect(browseButton, &QPushButton::clicked, this, &LabelTextBlock::_q_browseClicked);
    } else {
        browseButton = nullptr;
        textButtonLayout = nullptr;
    }

    mainLayout = new QVBoxLayout();
    mainLayout->setMargin(0);
    mainLayout->setSpacing(0);

    mainLayout->addWidget(label);
    if (browseButton) {
        textButtonLayout = new QHBoxLayout();
        textButtonLayout->setMargin(0);
        textButtonLayout->setSpacing(0);

        textButtonLayout->addWidget(text);
        textButtonLayout->addWidget(browseButton);

        mainLayout->addLayout(textButtonLayout);
    } else {
        mainLayout->addWidget(text);
    }

    setLayout(mainLayout);
}

LabelTextBlock::~LabelTextBlock() {
}

void LabelTextBlock::clear() {
    text->clear();
}

void LabelTextBlock::_q_browseClicked() {
    QString path = FileManager::instance()->openDir(tr("Browse"), FLAG_BROWSE, parentWidget());
    if (!path.isEmpty()) {
        text->setText(QDir::toNativeSeparators(path));
    }
}
