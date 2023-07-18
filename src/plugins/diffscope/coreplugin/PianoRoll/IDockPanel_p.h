#ifndef IDOCKPANELPRIVATE_H
#define IDOCKPANELPRIVATE_H

#include <QBoxLayout>
#include <QLabel>
#include <QToolBar>

#include "IDockPanel.h"

namespace Core {

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