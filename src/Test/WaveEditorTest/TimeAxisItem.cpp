
#include <QPainter>
#include <QDebug>
#include <QWheelEvent>
#include "WaveEditorView.h"
#include "TimeAxisItem.h"

TimeAxisItem::TimeAxisItem(QGraphicsItem *parent)
    : EventCaptureRectItem(parent),
    mTimecodeFont("sansserif", 10)
{
    QFontMetrics fm(mTimecodeFont);
    constexpr int rightMargin = 10;
    mTimecodeWidthMMSS = fm.horizontalAdvance("00:00") + rightMargin;
    mTimecodeWidthMMSSFF = fm.horizontalAdvance("00:00.00") + rightMargin;
}

TimeAxisItem::~TimeAxisItem()
{
}

void TimeAxisItem::SetTimeScale(uint64_t sampleCount, int sampleRate, int thumbnailWidth)
{
    mSampleCount = sampleCount;
    mSampleRate = sampleRate;
    mThumbnailWidth = thumbnailWidth;
}

void TimeAxisItem::eventSlot(QEvent *e, QPointF scenePos)
{
    switch(e->type()) {
    case QEvent::Wheel: {
        auto *event = static_cast<QWheelEvent*>(e);
        auto d = event->angleDelta().y();;
        auto hCenter = scenePos.x() / double(rect().width()) * mSampleCount;
        qDebug() << "ePos" << scenePos << "hCenter" << hCenter;
        static_cast<WaveEditorView*>(scene()->views()[0])->RequestZoom(pow(2, d / 240.0), hCenter, scenePos.x());
        break;
    }
    default:
        break;
    }
}

void TimeAxisItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Three LOD here. Advance the timecode on the base of 10, 5 or 2.
    // Find the highest viable detail level and use that.
    enum { UseMMSSFF, UseMMSS } timecodeKind = UseMMSSFF;

    double pxPerSecond = rect().width() / (mSampleCount / (double)mSampleRate);

    auto h = rect().height();
    
    // Longest timecode is MMSSFF, find out how long of audio can fit into its width
    double msPerTimecode = mTimecodeWidthMMSSFF / pxPerSecond * 1000.0;

    // If MMSSFF corresponds to too long of audio, use MMSS timecode.
    if(msPerTimecode > 1000.0) {
        msPerTimecode = mTimecodeWidthMMSS / pxPerSecond * 1000.0;
        timecodeKind = UseMMSS;
    }

    // Find out the magnitude
    int msMagnitudeBase10 = (int)log10(msPerTimecode);
    // Remove exponent and get coefficient
    uint32_t msAdvancement = pow(10, msMagnitudeBase10);
    double msCoefficient = msPerTimecode / msAdvancement;
    // Find out how many ms to advance after drawing each timecode
    if(msCoefficient < 2.0)
        msAdvancement *= 2;
    else if(msCoefficient < 5.0)
        msAdvancement *= 5;
    else
        msAdvancement *= 10;

    painter->setFont(mTimecodeFont);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::gray);

    // Draw background
    painter->drawRect(rect());
    painter->setPen(Qt::black);
    painter->drawLine(rect().bottomLeft(), rect().bottomRight());

    if(msAdvancement == 0)
        return;

    // Draw timecode
    float left = 0;
    for(int i = 0; left < rect().width(); i++) {
        QString timecode;
        uint32_t ms = i * msAdvancement;
        uint32_t s = ms / 1000;
        uint32_t m = s / 60;
        m %= 60;
        s %= 60;
        ms %= 1000;
        ms /= 10;

        painter->drawLine(left, 0.4 * h, left, h);

        switch(timecodeKind) {
        case UseMMSSFF:
            painter->drawText(left, h - 2, 
                              QString("%1:%2.%3")
                              .arg(m, 2, 10, QChar('0'))
                              .arg(s, 2, 10, QChar('0'))
                              .arg(ms, 2, 10, QChar('0')));
            break;
        case UseMMSS:
            painter->drawText(left, h - 2, 
                              QString("%1:%2")
                              .arg(m, 2, 10, QChar('0'))
                              .arg(s, 2, 10, QChar('0')));
            break;
        }
        left += msAdvancement * pxPerSecond / 1000.0;
    }
}
