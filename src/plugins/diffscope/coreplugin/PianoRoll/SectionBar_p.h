#ifndef SECTIONBARPRIVATE_H
#define SECTIONBARPRIVATE_H

#include "SectionBar.h"

#include "Window/IProjectWindow.h"

#include <QFontInfoEx.h>
#include <QPenInfo.h>
#include <QRectInfo.h>

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

        QFontInfoEx sectionNumber;
        QRectInfo sectionBackground;
        QFontInfoEx signatureNumber;
        QRectInfo signatureBackground;
        QFontInfoEx tempoNumber;
        QRectInfo tempoBackground;
        QPenInfo sectionLine;
        QPenInfo beatLine;
        QPenInfo tempoLine;

        const QsApi::MusicTimeline *timeline;

        int deltaX;

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

        enum class TempDataType {
            None,
            Tempo,
            TimeSignature,
            SectionNum,
        };

        union {
            int curSectionNum;
            int curTimeSignature;
            int curTempo;
        } hoverData;
        TempDataType hoverDataType;

        union {
            struct {
                int orgSectionNum;
                int targetSectionNum;
            };
            struct {
                int orgTimeSignature;
                int targetTimeSignature;
            };
            struct {
                int orgTempo;
                int targetTempo;
            };
        } dragData;
        TempDataType dragDataType;

        void updateLayout();
        void updateMouseArea(QMouseEvent *event);

        QCssValueMap styleData_helper() const;
        void setStyleData_helper(const QCssValueMap &map);

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

        void updatePlayHead();
    };

} // namespace Core

#endif // SECTIONBARPRIVATE_H
