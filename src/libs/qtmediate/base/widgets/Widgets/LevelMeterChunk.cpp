//
// Created by fluty on 2023/8/6.
//

#include "LevelMeterChunk.h"
#include "QPainter"
#include "QDebug"

class LevelMeterChunkPrivate {
public:
    LevelMeterChunk *q;

    explicit LevelMeterChunkPrivate(LevelMeterChunk *q) : q(q) {
        q->setAttribute(Qt::WA_StyledBackground);
    }
};

LevelMeterChunk::LevelMeterChunk(QWidget *parent) : QWidget(parent), d(new LevelMeterChunkPrivate(this)) {
}

LevelMeterChunk::~LevelMeterChunk() {
    delete d;
}

void LevelMeterChunk::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // Fill background
    painter.fillRect(rect(), QColor("#d9d9d9"));
//    qDebug() << rect();

    // Calculate and draw chunk
    auto levelWidth = rect().width() * level;
    if (level < 0.707946) {
        auto width = levelWidth;
        auto chunk1 = QRect(rect().left(), rect().top(), width, rect().height());
        painter.fillRect(chunk1, QColor("#709cff"));
    } else if (level < 0.891251) {
        auto width1 = rect().width() * 0.707946;
        auto chunk1 = QRect(rect().left(), rect().top(), width1, rect().height());
        painter.fillRect(chunk1, QColor("#709cff"));

        auto width2 = levelWidth - width1;
        auto chunk2 = QRect(rect().left() + width1, rect().top(), width2, rect().height());
        painter.fillRect(chunk2, QColor("#ffcc99"));
    } else if (level < 1) {
        auto width1 = rect().width() * 0.707946;
        auto chunk1 = QRect(rect().left(), rect().top(), width1, rect().height());
        painter.fillRect(chunk1, QColor("#709cff"));

        auto width2 = rect().width() * 0.891251 - width1;
        auto chunk2 = QRect(rect().left() + width1, rect().top(), width2, rect().height());
        painter.fillRect(chunk2, QColor("#ffcc99"));

        auto width3 = levelWidth - width1 - width2;
        auto chunk3 = QRect(rect().left() + width1 + width2, rect().top(), width3, rect().height());
        painter.fillRect(chunk3, QColor("#ff7c80"));
    } else {
        auto width1 = rect().width() * 0.707946;
        auto chunk1 = QRect(rect().left(), rect().top(), width1, rect().height());
        painter.fillRect(chunk1, QColor("#709cff"));

        auto width2 = rect().width() * 0.891251 - width1;
        auto chunk2 = QRect(rect().left() + width1, rect().top(), width2, rect().height());
        painter.fillRect(chunk2, QColor("#ffcc99"));

        auto width3 = rect().width() - width1 - width2;
        auto chunk3 = QRect(rect().left() + width1 + width2, rect().top(), width3, rect().height());
        painter.fillRect(chunk3, QColor("#ff7c80"));
    }

    painter.end();
    QWidget::paintEvent(event);
}

void LevelMeterChunk::setLevel(double l) {
    level = l;
    update();
}
