//
// Created by fluty on 2023/8/6.
//

#include "LevelMeterChunk.h"
#include "QPainter"
#include "QDebug"
#include "QTimer"

class LevelMeterChunkPrivate {
public:
    LevelMeterChunk *q;

    explicit LevelMeterChunkPrivate(LevelMeterChunk *q) : q(q) {
        q->setAttribute(Qt::WA_StyledBackground);
    }
};

LevelMeterChunk::LevelMeterChunk(QWidget *parent) : QWidget(parent), d(new LevelMeterChunkPrivate(this)) {
    timer = new QTimer(parent);
    timer->start(17);
    QObject::connect(timer, &QTimer::timeout, this, [=]() {
        double sum = 0;
        for (int i = 0; i < bufferSize; i++){
            sum += bufferPtr[i];
        }
        averageLevel = sum / bufferSize;
        // qDebug() << averageLevel;
        repaint();
    });
}

LevelMeterChunk::~LevelMeterChunk() {
    delete d;
    delete []bufferPtr;
}

void LevelMeterChunk::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // Fill background
    painter.fillRect(rect(), QColor("#d9d9d9"));
//    qDebug() << rect();

    // Calculate and draw chunk
    if (m_orientation == Qt::Horizontal) {
        auto levelLength = rect().width() * averageLevel;
        if (averageLevel < 0.707946) {
            auto width = levelLength;
            auto chunk1 = QRect(rect().left(), rect().top(), width, rect().height());
            painter.fillRect(chunk1, QColor("#709cff"));
        } else if (averageLevel < 0.891251) {
            auto width1 = rect().width() * 0.707946;
            auto chunk1 = QRect(rect().left(), rect().top(), width1, rect().height());
            painter.fillRect(chunk1, QColor("#709cff"));

            auto width2 = levelLength - width1;
            auto chunk2 = QRect(rect().left() + width1, rect().top(), width2, rect().height());
            painter.fillRect(chunk2, QColor("#ffcc99"));
        } else if (averageLevel < 1) {
            auto width1 = rect().width() * 0.707946;
            auto chunk1 = QRect(rect().left(), rect().top(), width1, rect().height());
            painter.fillRect(chunk1, QColor("#709cff"));

            auto width2 = rect().width() * 0.891251 - width1;
            auto chunk2 = QRect(rect().left() + width1, rect().top(), width2, rect().height());
            painter.fillRect(chunk2, QColor("#ffcc99"));

            auto width3 = levelLength - width1 - width2;
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
    } else {
        auto levelLength = int(rect().height() * averageLevel);
        auto length1 = int(rect().height() * 0.707946);
        auto length2 = int(rect().height() * 0.891251) - length1;
        auto length3 = rect().height() - length1 - length2;
        if (averageLevel < 0.707946) {
            auto height = levelLength;
            auto chunk1 = QRect(rect().left(), rect().height() - height, rect().width(), height);
            painter.fillRect(chunk1, QColor("#709cff"));
        } else if (averageLevel < 0.891251) {
            auto chunk1 = QRect(rect().left(), rect().height() - length1, rect().width(), length1);
            painter.fillRect(chunk1, QColor("#709cff"));

            auto height = levelLength - length1;
            auto chunk2 = QRect(rect().left(), rect().height() - levelLength, rect().width(), height);
            painter.fillRect(chunk2, QColor("#ffcc99"));
        } else if (averageLevel < 1) {
            auto chunk1 = QRect(rect().left(), rect().height() - length1, rect().width(), length1);
            painter.fillRect(chunk1, QColor("#709cff"));

            auto chunk2 = QRect(rect().left(), rect().height() - length1 - length2, rect().width(), length2);
            painter.fillRect(chunk2, QColor("#ffcc99"));

            auto height = levelLength - length1 - length2;
            auto chunk3 = QRect(rect().left(), rect().height() - levelLength, rect().width(), height);
            painter.fillRect(chunk3, QColor("#ff7c80"));
        } else {
            auto chunk1 = QRect(rect().left(), rect().height() - length1, rect().width(), length1);
            painter.fillRect(chunk1, QColor("#709cff"));

            auto chunk2 = QRect(rect().left(), rect().height() - length1 - length2, rect().width(), length2);
            painter.fillRect(chunk2, QColor("#ffcc99"));

            auto chunk3 = QRect(rect().left(), rect().top(), rect().width(), length3);
            painter.fillRect(chunk3, QColor("#ff7c80"));
        }
    }

    painter.end();
    QWidget::paintEvent(event);
}

void LevelMeterChunk::setLevel(double l) {
//    qDebug() << l;
    bufferPtr[bufferPos] = l;
    bufferPos++;
    if(bufferPos == bufferSize) {
        bufferPos = 0;
    }
//    update();
}

void LevelMeterChunk::setSampleRate(int sampleRate) {
}

void LevelMeterChunk::setBufferSize(int size) {
}

void LevelMeterChunk::initBuffer() {
    qDebug() << "init buffer" << bufferSize;
    bufferPtr = new double[bufferSize];
    resetBuffer();
}

void LevelMeterChunk::resetBuffer() {
    for (int i = 0; i < bufferSize; i++)
        bufferPtr[i] = 0;
    bufferPos = 0;
}

void LevelMeterChunk::setOrientation(Qt::Orientation orientation) {
    m_orientation = orientation;
    update();
}
