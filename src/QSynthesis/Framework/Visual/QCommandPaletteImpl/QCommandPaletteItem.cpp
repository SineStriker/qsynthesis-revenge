#include "QCommandPaletteItem.h"

#include <QEvent>
#include <QVariant>

QCommandPaletteItemWidget::QCommandPaletteItemWidget(QWidget *parent) : QWidget(parent) {
    item = nullptr;

    layout = new QGridLayout();
    layout->setMargin(0);
    layout->setSpacing(0);

    iconButton = new QPushButton();
    iconButton->setObjectName("icon-button");
    iconButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    leftTopLabel = new QLabel();
    leftTopLabel->setProperty("type", "label");
    leftTopLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    leftBottomLabel = new QLabel();
    leftBottomLabel->setProperty("type", "label");
    leftBottomLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    rightTopLabel = new QLabel();
    rightTopLabel->setProperty("type", "label");
    rightTopLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    rightBottomLabel = new QLabel();
    rightBottomLabel->setProperty("type", "label");
    rightBottomLabel->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);

    rightButton = new QPushButton();
    rightButton->setObjectName("right-button");
    rightButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    layout->addWidget(iconButton, 0, 0, 2, 1);
    layout->addWidget(leftTopLabel, 0, 1);
    layout->addWidget(leftBottomLabel, 1, 1);
    layout->addWidget(rightTopLabel, 0, 2);
    layout->addWidget(rightBottomLabel, 1, 2);
    layout->addWidget(rightButton, 0, 3, 2, 1);

    setLayout(layout);
}

QCommandPaletteItemWidget::~QCommandPaletteItemWidget() {
}

QString QCommandPaletteItemWidget::leftTopText() const {
    return leftTopLabel->text();
}

void QCommandPaletteItemWidget::setLeftTopText(const QString &text) const {
    leftTopLabel->setText(text);
}

QString QCommandPaletteItemWidget::leftBottomText() const {
    return leftBottomLabel->text();
}

void QCommandPaletteItemWidget::setLeftBottomText(const QString &text) const {
    leftBottomLabel->setText(text);
}

QString QCommandPaletteItemWidget::rightTopText() const {
    return rightTopLabel->text();
}

void QCommandPaletteItemWidget::setRightTopText(const QString &text) const {
    rightTopLabel->setText(text);
}

QString QCommandPaletteItemWidget::rightBottomText() const {
    return rightBottomLabel->text();
}

void QCommandPaletteItemWidget::setRightBottomText(const QString &text) const {
    rightBottomLabel->setText(text);
}

bool QCommandPaletteItemWidget::event(QEvent *event) {
    switch (event->type()) {
    case QEvent::LayoutRequest:
        if (item) {
            item->setSizeHint(sizeHint());
        }
        break;
    default:
        break;
    }
    return QWidget::event(event);
}
