#ifndef CHORUSKIT_SECTIONWIDGET_H
#define CHORUSKIT_SECTIONWIDGET_H

#include "PianoRoll/SectionBar.h"

#include <QPixelSize.h>

#include <QGraphicsScene>

namespace Core {

    class CanvasView;

    class CanvasScene;

    namespace Internal {

        class SectionWidget : public SectionBar {
            Q_OBJECT
            Q_PROPERTY(QPixelSize preferredHeight READ preferredHeight WRITE setPreferredHeight)
        public:
            explicit SectionWidget(IProjectWindow *iWin, QWidget *parent = nullptr);
            ~SectionWidget();

            void initialize() override;
            void extensionInitialized() override;

        public:
            QPixelSize preferredHeight() const;
            void setPreferredHeight(const QPixelSize &height);

            QSize sizeHint() const override;

        protected:
            void paintEvent(QPaintEvent *event) override;

        private:
            int m_preferredHeight;

            CanvasView *view;
            CanvasScene *scene;

            void _q_viewMoved(const QPointF &pos, const QPointF &oldPos);
            void _q_viewResized(const QSizeF &size, const QSizeF &newSize);
            void _q_currentWidthChanged(int w);
            void _q_currentSnapChanged(int s);
        };
    }

}


#endif // CHORUSKIT_SECTIONWIDGET_H
