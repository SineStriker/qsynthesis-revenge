#ifndef SECTIONBARPRIVATE_H
#define SECTIONBARPRIVATE_H

#include "SectionBar.h"

#include "Window/IProjectWindow.h"

#include <QLineStyle.h>
#include <QRectStyle.h>
#include <QTypeFace.h>

namespace Core {

    class SectionBarPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(SectionBar)
    public:
        SectionBarPrivate();
        virtual ~SectionBarPrivate();

        void init();

        SectionBar *q_ptr;
        bool pressed;

        int curWidth;
        int startPos;

        QTypeFace sectionNumber;
        QRectStyle sectionBackground;
        QTypeFace signatureNumber;
        QRectStyle signatureBackground;
        QTypeFace tempoNumber;
        QRectStyle tempoBackground;
        QLineStyle sectionLine;
        QLineStyle beatLine;
        QLineStyle tempoLine;

        const QsApi::MusicTimeline *timeline;

        struct TimeSig {
            QRect rect;
            int section;
            int tick;
            QsApi::MusicTimeSignature value;
        };
        QMap<int, TimeSig> timeSignatureBlocks;

        struct Tempo {
            QRect rect;
            int tick;
            double value;
        };
        QMap<int, Tempo> tempoBlocks;

        void updateLayout();
        void updateMouseArea(QMouseEvent *event);
    };

} // namespace Core

#endif // SECTIONBARPRIVATE_H
