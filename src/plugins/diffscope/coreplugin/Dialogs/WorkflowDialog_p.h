#ifndef WORKFLOWDIALOGPRIVATE_H
#define WORKFLOWDIALOGPRIVATE_H

#include "WorkflowDialog.h"

#include <QLabel>
#include <QPropertyAnimation>

#include "CTabButton.h"
#include "QMEqualBoxLayout.h"

namespace Core {

    class WorkflowDialogPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(WorkflowDialog)
    public:
        WorkflowDialogPrivate();
        ~WorkflowDialogPrivate();

        void init();

        void reloadStrings();

        WorkflowDialog *q_ptr;

        CTabButton *prevButton, *nextButton, *cancelButton;
        QMEqualBoxLayout *buttonsLayout;

        bool hasPrev, hasNext;

        QWidget *widget;

        QFrame *container;
        QVBoxLayout *mainLayout;

        QPropertyAnimation *m_animation;
        QPropertyAnimation *m_animation2;
        QLabel *m_label;

        void refreshPrevButton();
        void refreshNextButton();

        void prepareTransition();
        virtual void doPrevTransition();
        virtual void doNextTransition();

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        void _q_prevClicked();
        void _q_nextClicked();
    };

}

#endif // WORKFLOWDIALOGPRIVATE_H