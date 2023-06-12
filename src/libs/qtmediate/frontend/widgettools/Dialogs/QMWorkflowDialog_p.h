#ifndef QMWORKFLOWDIALOG_P_H
#define QMWORKFLOWDIALOG_P_H

#include "QMWorkflowDialog.h"

#include <QLabel>
#include <QPropertyAnimation>

#include "CTabButton.h"
#include "QMEqualBoxLayout.h"

class QMWorkflowDialogContainer;

class QMWorkflowDialogPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(QMWorkflowDialog)
public:
    QMWorkflowDialogPrivate();
    ~QMWorkflowDialogPrivate();

    void init();

    QMWorkflowDialog *q_ptr;

    CTabButton *prevButton, *nextButton, *finishButton, *okButton, *cancelButton, *helpButton;
    QMEqualBoxLayout *buttonsLayout;

    QMWorkflowPage *widget;

    QMWorkflowDialogContainer *container;
    QVBoxLayout *mainLayout;

    QPropertyAnimation *m_animation;
    QPropertyAnimation *m_animation2;
    QLabel *m_label;

    QMap<QMWorkflowPage::Button, QAbstractButton *> buttons;

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

    void _q_buttonsChanged(QMWorkflowPage::Buttons buttons);
    void _q_buttonTextChanged(QMWorkflowPage::Button which, const QString &text);
    void _q_buttonEnabledChanged(QMWorkflowPage::Button which, bool enabled);
};

#endif // QMWORKFLOWDIALOG_P_H
