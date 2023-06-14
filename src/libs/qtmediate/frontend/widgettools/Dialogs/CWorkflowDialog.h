#ifndef CWORKFLOWDIALOG_H
#define CWORKFLOWDIALOG_H

#include <QDialog>

#include "CWorkflowPage.h"

class CWorkflowDialogPrivate;

class QMWTOOLS_EXPORT CWorkflowDialog : public QDialog {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CWorkflowDialog)
public:
    explicit CWorkflowDialog(QWidget *parent = nullptr);
    ~CWorkflowDialog();

public:
    CWorkflowPage *page() const;
    CWorkflowPage *takePage();
    void setPage(CWorkflowPage *w);

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
    CWorkflowDialog(CWorkflowDialogPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<CWorkflowDialogPrivate> d_ptr;
};

#endif // CWORKFLOWDIALOG_H
