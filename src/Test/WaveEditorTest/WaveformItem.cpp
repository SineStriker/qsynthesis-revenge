
#include "WaveformItem.h"

#include <QDebug>
#include <QPainter>
#include <QStyleOptionGraphicsItem>

WaveformItem::WaveformItem(QGraphicsItem *parent) {
    setFlag(ItemUsesExtendedStyleOption, true);
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

void WaveformItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *widget) {
    auto expRect = option->exposedRect;
    double stepping = double(mTotalSize) / rect().width();
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

                    painter->drawLine(i, 150 - saMax * 150, i, 150 - saMin * 150);
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
                        painter->drawLine(i, 150 - saMax * 150, i, 150 - saMin * 150);
                    }
                }
            }
            qInfo() << expRect.width() * readSize << "Points used in render";
            break;
        }

        case UseDecoder: {

            break;
        }
    }
}

void WaveformItem::eventSlot(QEvent *e, QPointF itemPos) {
}
