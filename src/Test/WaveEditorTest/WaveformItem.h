
#pragma once

#include <QVector>
#include "EventCapture.h"

#include "Api/IAudioDecoder.h"

/**
 * @brief WaveformItem reads features from the cached coarse pkpks and draw waveforms onto the view
 *        When given a decoder item, it will use the decoder
 */
class WaveformItem : public EventCaptureRectItem {
public:
    WaveformItem(QGraphicsItem *parent = nullptr);
    ~WaveformItem() {}

    void SetFeatureVector(QVector<QPair<float, float>> featureVec);
    void SetDecoder(IAudioDecoder *decoder, uint64_t totalSamples);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = nullptr) override;

    virtual void eventSlot(QEvent *e, QPointF itemPos) override;

private:
    // Feature vector ref
    QVector<QPair<float, float>> mFeatureVector;
    IAudioDecoder *mDecoder;

    uint64_t mTotalSize;

    enum { UseFeatureVector, UseDecoder } mRunningMode;
};
	
