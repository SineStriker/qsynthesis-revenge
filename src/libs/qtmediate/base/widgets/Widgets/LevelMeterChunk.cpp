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
        for (int i = 0; i < m_bufferSize; i++){
            sum += bufferPtr[i];
        }
        averageLevel = sum / m_bufferSize;
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
    painter.fillRect(rect(), m_colorBackground);
//    qDebug() << rect();

    // Calculate and draw chunk
    if (m_orientation == Qt::Horizontal) {
        auto rectWidth = rect().width();
        auto rectLeft = rect().left();
        auto rectTop = rect().top();
        auto rectHeight = rect().height();

        auto levelLength = int(rectWidth * averageLevel);
        auto lengthSafe = int(rectWidth * m_safeThreshold);
        auto lengthWarn = int(rectWidth * m_warnThreshold) - lengthSafe;
        auto lengthCritical = rect().width() - lengthSafe - lengthWarn;

        auto leftSafe = rectLeft;
        auto leftWarn = rectLeft + lengthSafe;
        auto leftCritical = rectLeft + lengthSafe + lengthWarn;

        if (averageLevel < m_safeThreshold) {
            auto width = levelLength;
            auto chunk1 = QRect(leftSafe, rectTop, width, rectHeight);
            painter.fillRect(chunk1, m_colorSafe);
        } else if (averageLevel < m_warnThreshold) {
            auto chunk1 = QRect(leftSafe, rectTop, lengthSafe, rectHeight);
            painter.fillRect(chunk1, m_colorSafe);

            auto width2 = levelLength - lengthSafe;
            auto chunk2 = QRect(leftWarn, rectTop, width2, rectHeight);
            painter.fillRect(chunk2, m_colorWarn);
        } else if (averageLevel < 1) {
            auto chunk1 = QRect(leftSafe, rectTop, lengthSafe, rectHeight);
            painter.fillRect(chunk1, m_colorSafe);

            auto chunk2 = QRect(leftWarn, rectTop, lengthWarn, rectHeight);
            painter.fillRect(chunk2, m_colorWarn);

            auto width3 = levelLength - lengthSafe - lengthWarn;
            auto chunk3 = QRect(leftCritical, rectTop, width3, rectHeight);
            painter.fillRect(chunk3, m_colorCritical);
        } else {
            auto chunk1 = QRect(leftSafe, rectTop, lengthSafe, rectHeight);
            painter.fillRect(chunk1, m_colorSafe);

            auto chunk2 = QRect(leftWarn, rectTop, lengthWarn, rectHeight);
            painter.fillRect(chunk2, m_colorWarn);

            auto chunk3 = QRect(leftCritical, rectTop, lengthCritical, rectHeight);
            painter.fillRect(chunk3, m_colorCritical);
        }
    } else {
        auto rectLeft = rect().left();
        auto rectHeight = rect().height();
        auto rectWidth = rect().width();

        auto levelLength = int(rectHeight * averageLevel);
        auto lengthSafe = int(rectHeight * m_safeThreshold);
        auto lengthWarn = int(rectHeight * m_warnThreshold) - lengthSafe;
        auto lengthCritical = rectHeight - lengthSafe - lengthWarn;

        auto topSafe = rectHeight - lengthSafe;
        auto topWarn = rectHeight - lengthSafe - lengthWarn;
        auto topCritical = rect().top();

        if (averageLevel < m_safeThreshold) {
            auto height = levelLength;
            auto top1 = rectHeight - height;
            auto chunkSafe = QRect(rectLeft, top1, rectWidth, height);
            painter.fillRect(chunkSafe, m_colorSafe);
        } else if (averageLevel < m_warnThreshold) {
            auto chunkSafe = QRect(rectLeft, topSafe, rectWidth, lengthSafe);
            painter.fillRect(chunkSafe, m_colorSafe);

            auto height = levelLength - lengthSafe;
            auto top2 = rectHeight - levelLength;
            auto chunkWarn = QRect(rectLeft, top2, rectWidth, height);
            painter.fillRect(chunkWarn, m_colorWarn);
        } else if (averageLevel < 1) {
            auto chunkSafe = QRect(rectLeft, topSafe, rectWidth, lengthSafe);
            painter.fillRect(chunkSafe, m_colorSafe);

            auto chunkWarn = QRect(rectLeft, topWarn, rectWidth, lengthWarn);
            painter.fillRect(chunkWarn, m_colorWarn);

            auto height = levelLength - lengthSafe - lengthWarn;
            auto top3 = rectHeight - levelLength;
            auto chunk3 = QRect(rectLeft, top3, rectWidth, height);
            painter.fillRect(chunk3, m_colorCritical);
        } else {
            auto chunkSafe = QRect(rectLeft, topSafe, rectWidth, lengthSafe);
            painter.fillRect(chunkSafe, m_colorSafe);

            auto chunkWarn = QRect(rectLeft, topWarn, rectWidth, lengthWarn);
            painter.fillRect(chunkWarn, m_colorWarn);

            auto chunkCritical = QRect(rectLeft, topCritical, rectWidth, lengthCritical);
            painter.fillRect(chunkCritical, m_colorCritical);
        }
    }

    painter.end();
    QWidget::paintEvent(event);
}

void LevelMeterChunk::readSample(double sample) {
//    qDebug() << l;
    bufferPtr[bufferPos] = sample;
    bufferPos++;
    if(bufferPos == m_bufferSize) {
        bufferPos = 0;
    }
//    update();
}

//void LevelMeterChunk::setSampleRate(int sampleRate) {
//}

void LevelMeterChunk::setBufferSize(int size) {
}

void LevelMeterChunk::initBuffer(int bufferSize) {
    m_bufferSize = bufferSize;
    bufferPtr = new double[m_bufferSize];
    resetBuffer();
}

void LevelMeterChunk::resetBuffer() {
    for (int i = 0; i < m_bufferSize; i++)
        bufferPtr[i] = 0;
    bufferPos = 0;
}

void LevelMeterChunk::setOrientation(Qt::Orientation orientation) {
    m_orientation = orientation;
    update();
}
