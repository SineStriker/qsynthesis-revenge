#ifndef IDOCKPANELPRIVATE_H
#define IDOCKPANELPRIVATE_H

#include <QBoxLayout>
#include <QLabel>
#include <QToolBar>

#include "IDockPanel.h"

namespace Core {

    class CaptionLabel : public QLabel {
    public:
        explicit CaptionLabel(IDockPanel *sp, QWidget *parent = nullptr);
        ~CaptionLabel();

        IDockPanel *s;

    protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override;
    };

    class IDockPanelPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(IDockPanel)
    public:
        IDockPanelPrivate();
        virtual ~IDockPanelPrivate();

        void init();

        IDockPanel *q_ptr;

        QWidget *captionWidget;

        QLabel *captionLabel;
        QToolBar *toolBar;

        QHBoxLayout *captionLayout;
        QVBoxLayout *mainLayout;

    private:
        void _q_closeActionTriggered();
        void _q_menuActionTriggered();
    };

}

#endif // IDOCKPANELPRIVATE_H