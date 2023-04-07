#ifndef WORKFLOWDIALOG_H
#define WORKFLOWDIALOG_H

#include <QDialog>

#include "coreplugin/CoreGlobal.h"

namespace Core {

    class WorkflowDialogPrivate;

    class CORE_EXPORT WorkflowDialog : public QDialog {
        Q_OBJECT
        Q_DECLARE_PRIVATE(WorkflowDialog)
    public:
        explicit WorkflowDialog(QWidget *parent = nullptr);
        ~WorkflowDialog();

    public:
        bool prevEnabled() const;
        void setPrevEnabled(bool enabled);

        bool nextEnabled() const;
        void setNextEnabled(bool enabled);

        QWidget *widget() const;
        QWidget *takeWidget();
        void setWidget(QWidget *w);

    protected:
        virtual void prev();
        virtual void next();
        virtual void finish();

    signals:
        void aboutToPrev();
        void aboutToNext();
        void finished();

    protected:
        WorkflowDialog(WorkflowDialogPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<WorkflowDialogPrivate> d_ptr;
    };

}

#endif // WORKFLOWDIALOG_H