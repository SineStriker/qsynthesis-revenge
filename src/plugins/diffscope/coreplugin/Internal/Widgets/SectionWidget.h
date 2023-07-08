#ifndef CHORUSKIT_SECTIONWIDGET_H
#define CHORUSKIT_SECTIONWIDGET_H

#include "PianoRoll/SectionBar.h"

#include <QPixelSize.h>

#include <QGraphicsScene>

namespace Core::Internal {

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

        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        int m_preferredHeight;

        QGraphicsView *view;
        QGraphicsScene *scene;
    };

}


#endif // CHORUSKIT_SECTIONWIDGET_H
