
#pragma once

#include <QGraphicsRectItem>
#include <QFont>

class TimeAxisItem : public QGraphicsRectItem
{
public:
    TimeAxisItem(QGraphicsItem *parent = nullptr);
    ~TimeAxisItem();

    // We currently use the width of overview thumbnail as the total horizontal length, we then need
    // to set time scale (how wide is the thumbnail and how long is the audio) so the time axis knows
    // how scaling works
    void SetTimeScale(uint64_t sampleCount, int sampleRate, int thumbnailWidth);
    void SetThumbnailWidth(int thumbnailWidth) { mThumbnailWidth = thumbnailWidth; }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    uint64_t mSampleCount;
    int mSampleRate;
    int mThumbnailWidth;

    // Used to determine how fine the granularity of time axis is
    int mTimecodeWidthMMSS;
    int mTimecodeWidthMMSSFF;

    QFont mTimecodeFont;
};