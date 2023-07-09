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
        QPoint pressedPos;

        int curWidth;
        int curSnap;
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

        int deltaX;
        int curBarNumber;

        struct TimeSig {
            QRect rect;
            int section;
            int tick;
            QsApi::MusicTimeSignature value;
        };
        QMap<int, TimeSig> timeSignatureBlocks;
        int curTimeSignature;

        struct Tempo {
            QRect rect;
            int tick;
            double value;
        };
        QMap<int, Tempo> tempoBlocks;
        int curTempo;
        int targetTempo;

        void updateLayout();
        void updateMouseArea(QMouseEvent *event);

        QTypeMap styleData_helper() const;
        void setStyleData_helper(const QTypeMap &map);

        void edit_sectionClicked();
        void edit_sectionRightClicked();
        void edit_sectionDoubleClicked();

        void edit_timeSignatureClicked();
        void edit_timeSignatureRightClicked();
        void edit_timeSignatureDoubleClicked();

        void edit_tempoClicked();
        void edit_tempoRightClicked();
        void edit_tempoDoubleClicked();
        void edit_tempoMoved();

        void edit_blankRightClicked();
    };

} // namespace Core

#endif // SECTIONBARPRIVATE_H
