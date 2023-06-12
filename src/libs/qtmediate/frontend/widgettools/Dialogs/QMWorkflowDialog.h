#ifndef QMWORKFLOWDIALOG_H
#define QMWORKFLOWDIALOG_H

#include <QDialog>

#include "QMWorkflowPage.h"

class QMWorkflowDialogPrivate;

class QMWTOOLS_EXPORT QMWorkflowDialog : public QDialog {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMWorkflowDialog)
public:
    explicit QMWorkflowDialog(QWidget *parent = nullptr);
    ~QMWorkflowDialog();

public:
    QMWorkflowPage *page() const;
    QMWorkflowPage *takePage();
    void setPage(QMWorkflowPage *w);

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
    QMWorkflowDialog(QMWorkflowDialogPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<QMWorkflowDialogPrivate> d_ptr;
};

#endif // QMWORKFLOWDIALOG_H
