#ifndef SECTIONBAR_H
#define SECTIONBAR_H

#include <QFrame>

#include <MusicUtil/MusicTimeline.h>
#include <QCssValueMap.h>

#include "coreplugin/CoreGlobal.h"
#include "coreplugin/Interfaces/IPianoRollComponent.h"

namespace Core {
    /**
     *
     * StyleData:
     *  - sectionNumber: QTypeFace
     *  - signatureNumber: QTypeFace
     *  - signatureBackground: QRectStyle
     *  - tempoNumber: QTypeFace
     *  - tempoBackground: QRectStyle
     *  - sectionLine: QLineStyle
     *  - beatLine: QLineStyle
     *  - tempoLine: QLineStyle
     *
     */

    class SectionBarPrivate;

    class CORE_EXPORT SectionBar : public QFrame, public IPianoRollComponent {
        Q_OBJECT
        Q_PROPERTY(QCssValueMap styleData READ styleData WRITE setStyleData)
        Q_DECLARE_PRIVATE(SectionBar)
    public:
        explicit SectionBar(IProjectWindow *iWin, QWidget *parent = nullptr);
        ~SectionBar();

        void initialize() override;
        void extensionInitialized() override;

        QCssValueMap styleData() const;
        void setStyleData(const QCssValueMap &map);

    public:
        int startPos() const;
        void setStartPos(int tick);

        int currentWidth() const;
        void setCurrentWidth(int width);

        int currentSnap() const;
        void setCurrentSnap(int snap);

        void changeTimeSignatureAt(int bar);
        void changeTempoAt(int tick);

    signals:

    protected:
        void paintEvent(QPaintEvent *event) override;

        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;
        void mouseDoubleClickEvent(QMouseEvent *event) override;

        void enterEvent(QEvent *event) override;
        void leaveEvent(QEvent *event) override;

    protected:
        SectionBar(SectionBarPrivate &d, IProjectWindow *iWin, QWidget *parent = nullptr);

        QScopedPointer<SectionBarPrivate> d_ptr;
    };

} // namespace Core

#endif // SECTIONBAR_H
