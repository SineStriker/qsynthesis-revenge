#ifndef WORKFLOWDIALOGPRIVATE_H
#define WORKFLOWDIALOGPRIVATE_H

#include "../WorkflowDialog.h"

#include <QLabel>
#include <QPropertyAnimation>

#include "CTabButton.h"
#include "QMEqualBoxLayout.h"

namespace QsApi {

    class WorkflowDialogContainer;

    class WorkflowDialogPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(WorkflowDialog)
    public:
        WorkflowDialogPrivate();
        ~WorkflowDialogPrivate();

        void init();

        WorkflowDialog *q_ptr;

        CTabButton *prevButton, *nextButton, *finishButton, *okButton, *cancelButton, *helpButton;
        QMEqualBoxLayout *buttonsLayout;

        WorkflowPage *widget;

        WorkflowDialogContainer *container;
        QVBoxLayout *mainLayout;

        QPropertyAnimation *m_animation;
        QPropertyAnimation *m_animation2;
        QLabel *m_label;

        QMap<WorkflowPage::Button, QAbstractButton *> buttons;

        void prepareTransition();
        virtual void doPrevTransition();
        virtual void doNextTransition();

    protected:
        bool eventFilter(QObject *obj, QEvent *event) override;

    private:
        void _q_prevClicked();
        void _q_nextClicked();
        void _q_finishClicked();
        void _q_helpClicked();

        void _q_buttonsChanged(WorkflowPage::Buttons buttons);
        void _q_buttonTextChanged(WorkflowPage::Button which, const QString &text);
        void _q_buttonEnabledChanged(WorkflowPage::Button which, bool enabled);
    };

}

#endif // WORKFLOWDIALOGPRIVATE_H