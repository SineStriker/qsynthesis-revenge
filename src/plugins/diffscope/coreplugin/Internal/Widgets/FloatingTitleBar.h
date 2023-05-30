#ifndef CHORUSKIT_FLOATINGTITLEBAR_H
#define CHORUSKIT_FLOATINGTITLEBAR_H

#include <QFrame>
#include <QHBoxLayout>

#include "Widgets/SynthVSplitter.h"

#include <CPushButton.h>
#include <CToolButton.h>
#include <QPixelSize.h>

namespace Core::Internal {

    class FloatingTitleBarButton : public CPushButton {
        Q_OBJECT
        Q_PROPERTY_DECLARE(QColor, hintColor, HintColor)
        Q_PROPERTY_DECLARE(QPixelSize, hintHeight, HintHeight)
    public:
        explicit FloatingTitleBarButton(QWidget *parent = nullptr);
        ~FloatingTitleBarButton();

    protected:
        void mousePressEvent(QMouseEvent *event) override;
        void mouseMoveEvent(QMouseEvent *event) override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    private:
        bool m_pressed;
        QWidget *m_hintWidget;
        int m_lastIndex;

        QsApi::SynthVSplitterHandle *m_curHandle;
        QsApi::SynthVSplitter *m_splitter;
    };

    class FloatingTitleBar : public QFrame {
        Q_OBJECT
        Q_LAYOUT_PROPERTY_DELCARE
    public:
        explicit FloatingTitleBar(QWidget *parent = nullptr);
        ~FloatingTitleBar();

    public:
        QAbstractButton *foldButton() const;
        QAbstractButton *titleButton() const;
        QAbstractButton *closeButton() const;

        QWidget *titleBar() const;
        QWidget *takeTitleBar();
        void setTitleBar(QWidget *w);

    private:
        CToolButton *m_foldButton;
        FloatingTitleBarButton *m_titleButton;
        CToolButton *m_closeButton;

        QHBoxLayout *m_layout;
        QWidget *m_titleBar;
    };

}



#endif // CHORUSKIT_FLOATINGTITLEBAR_H
