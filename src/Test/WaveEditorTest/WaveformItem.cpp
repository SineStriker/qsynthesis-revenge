#include "NAudio/WaveStream.h"
#include "WaveformItem.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

WaveformItem::WaveformItem(QGraphicsItem *parent) {
    setFlag(ItemUsesExtendedStyleOption, true);
    mSkip = 0;
}

void WaveformItem::SetFeatureVector(QVector<QPair<float, float>> featureVec) {
    mFeatureVector = featureVec;
    mTotalSize = featureVec.size();
    mRunningMode = UseFeatureVector;
}

void WaveformItem::SetDecoder(IAudioDecoder *decoder, uint64_t totalSamples) {
    mDecoder = decoder;
    mTotalSize = totalSamples;
    mRunningMode = UseDecoder;
}

void WaveformItem::SetSkipLevel(int skipLevel) {
    /*  
        -3: 1sa/pt (direct)
        -2: 4sa/pt (skip)
        -1: 20sa/pt (skip)
    */
    skipLevel *= -1;
    mSkip = ceil(100 / pow(5, skipLevel));
}

void WaveformItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget) {
    auto expRect = option->exposedRect;
    double stepping = double(mTotalSize) / rect().width();
    auto h_2 = rect().height() / 2;
    switch (mRunningMode) {
        case UseFeatureVector: {
            float readSize = (float) mTotalSize / rect().width();
            float from, to;
            painter->setBrush(Qt::black);
            painter->drawRect(expRect);
            painter->setPen(Qt::red);
            painter->setBrush(Qt::red);
            if (readSize < 1) {
                // TODO
                for (int i = expRect.left(); i < expRect.right(); i++) {
                    auto pair = mFeatureVector[i * readSize];
                    float saMax = pair.first, saMin = pair.second;
                    
                    //painter->drawRect(
                    //    QRectF(QPointF(i, 150 - saMax * 150),
                    //           QPointF(i, 150 - saMin * 150)));

                    painter->drawLine(i, h_2 - saMax * h_2, i, h_2 - saMin * h_2);
                }
            } else {
                for (int i = expRect.left(); i < expRect.right(); i++) {
                    float saMax = 0.0f, saMin = 0.0f;
                    from = round(i * readSize);
                    to = std::min((uint64_t) round((i + 1) * readSize), mTotalSize);
                    for (int j = from; j < to; j++) {
                        auto pair = mFeatureVector[j];
                        if (pair.first > saMax)
                            saMax = pair.first;
                        if (pair.second < saMin)
                            saMin = pair.second;
                        painter->drawLine(i, h_2 - saMax * h_2, i, h_2 - saMin * h_2);
                    }
                }
            }
            qInfo() << expRect.width() * readSize << "Points used in render";
            break;
        }

        case UseDecoder: {
            QPointF lastPoint(expRect.left(), 0.0), currPoint(expRect.left(), 0.0);
            uint64_t from = double(expRect.left()) / rect().width() * mTotalSize;
            double pxAdvance = double(rect().width()) / mTotalSize * mSkip;
            int offset = 0, points = 0;
            float buffer[100];

            mDecoder->Seek(from * 4, WaveStream::Begin);
            mDecoder->Read(buffer, 0, 100);
            lastPoint.setY(h_2 + buffer[0] * h_2);

            painter->setBrush(Qt::black);
            painter->drawRect(expRect);
            painter->setPen(Qt::red);

//            if (mSkip == 0) {
                while (currPoint.x() < expRect.right()) {
                    if (offset >= 100) {
                        mDecoder->Read(buffer, 0, 100);
                        offset %= 100;
                    }

                    currPoint.setY(h_2 + buffer[offset] * h_2);
                    currPoint.rx() += pxAdvance;

                    painter->drawLine(lastPoint, currPoint);

                    offset += mSkip;
                    lastPoint = currPoint;
                    points++;
                }
//            } else {
//                
//            }
                qInfo() << points << "Points used in render";
            break;
        }
    }
}

void WaveformItem::eventSlot(QEvent *e, QPointF itemPos) {
}
