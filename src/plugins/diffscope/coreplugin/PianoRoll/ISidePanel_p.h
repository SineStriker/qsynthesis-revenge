#ifndef ISIDEPANELPRIVATE_H
#define ISIDEPANELPRIVATE_H

#include <QBoxLayout>
#include <QLabel>
#include <QToolBar>

#include "ISidePanel.h"

namespace Core {

    class CaptionLabel : public QLabel {
    public:
        explicit CaptionLabel(ISidePanel *sp, QWidget *parent = nullptr);
        ~CaptionLabel();

        ISidePanel *s;

    protected:
        void mouseDoubleClickEvent(QMouseEvent *event) override;
    };

    class ISidePanelPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ISidePanel)
    public:
        ISidePanelPrivate();
        virtual ~ISidePanelPrivate();

        void init();

        ISidePanel *q_ptr;

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

#endif // ISIDEPANELPRIVATE_H