#ifndef WORKFLOWDIALOGPRIVATE_H
#define WORKFLOWDIALOGPRIVATE_H

#include "WorkflowDialog.h"

#include <QLabel>
#include <QPropertyAnimation>

#include "CTabButton.h"
#include "QMEqualBoxLayout.h"

namespace Core {

    class PosWrapper : public QObject {
        Q_OBJECT
        Q_PROPERTY(int x READ x WRITE setX)
        Q_PROPERTY(int y READ y WRITE setY)

    public:
        explicit PosWrapper(QObject *parent = nullptr) : QObject(parent) {
        }

        inline int x() const {
            return w->x();
        }

        inline void setX(int x) {
            w->move(x, w->y());
        }

        inline int y() const {
            return w->y();
        }
        inline void setY(int y) {
            w->move(w->x(), y);
        }

        QWidget *w;
    };

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

        PosWrapper *m_pos1;
        PosWrapper *m_pos2;

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