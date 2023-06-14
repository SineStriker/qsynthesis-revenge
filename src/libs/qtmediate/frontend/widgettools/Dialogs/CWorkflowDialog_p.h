#ifndef CWORKFLOWDIALOG_P_H
#define CWORKFLOWDIALOG_P_H

#include "CWorkflowDialog.h"

#include <QLabel>
#include <QPropertyAnimation>

#include "CTabButton.h"
#include "QMEqualBoxLayout.h"

class QMWorkflowDialogContainer;

class CWorkflowDialogPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CWorkflowDialog)
public:
    CWorkflowDialogPrivate();
    ~CWorkflowDialogPrivate();

    void init();

    CWorkflowDialog *q_ptr;

    CTabButton *prevButton, *nextButton, *finishButton, *okButton, *cancelButton, *helpButton;
    QMEqualBoxLayout *buttonsLayout;

    CWorkflowPage *widget;

    QMWorkflowDialogContainer *container;
    QVBoxLayout *mainLayout;

    QPropertyAnimation *m_animation;
    QPropertyAnimation *m_animation2;
    QLabel *m_label;

    QMap<CWorkflowPage::Button, QAbstractButton *> buttons;

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

    void _q_buttonsChanged(CWorkflowPage::Buttons buttons);
    void _q_buttonTextChanged(CWorkflowPage::Button which, const QString &text);
    void _q_buttonEnabledChanged(CWorkflowPage::Button which, bool enabled);
};

#endif // CWORKFLOWDIALOG_P_H
