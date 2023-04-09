#ifndef WORKFLOWDIALOG_H
#define WORKFLOWDIALOG_H

#include <QDialog>

#include "WorkflowPage.h"

namespace Core {

    class WorkflowDialogPrivate;

    class CKAPPCORE_API WorkflowDialog : public QDialog {
        Q_OBJECT
        Q_DECLARE_PRIVATE(WorkflowDialog)
    public:
        explicit WorkflowDialog(QWidget *parent = nullptr);
        ~WorkflowDialog();

    public:
        WorkflowPage *page() const;
        WorkflowPage *takePage();
        void setPage(WorkflowPage *w);

    protected:
        virtual bool prev();
        virtual bool next();
        virtual bool finish();

    signals:
        void aboutToPrev();
        void aboutToNext();
        void finished();

        void helpRequested();

    protected:
        WorkflowDialog(WorkflowDialogPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<WorkflowDialogPrivate> d_ptr;
    };

}

#endif // WORKFLOWDIALOG_H