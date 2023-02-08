#ifndef IMPORTDIALOGPRIVATE_H
#define IMPORTDIALOGPRIVATE_H

#include "../ImportDialog.h"

#include "CPushButton.h"
#include "QLinkList.h"

#include <QAction>
#include <QButtonGroup>
#include <QLabel>
#include <QVBoxLayout>

class ImportDialogPrivate {
    Q_DECLARE_PUBLIC(ImportDialog)
public:
    ImportDialogPrivate();
    ~ImportDialogPrivate();

    void init();

    ImportDialog *q_ptr;

    // Options
    ImportDialog::ImportOptions opt;

    // Results
    QList<int> trackIndexs;
    QTextCodec *codec;

    // UI
    QVBoxLayout *layout;
    QVBoxLayout *boxesLayout;
    QHBoxLayout *buttonsLayout;

    QLabel *lbCaption;
    CPushButton *btnCancel, *btnOK;

    QAction *okAction;
    QButtonGroup *boxGroup;

    QLinkList<QAbstractButton *> queue;
    QHash<QAbstractButton *, QLinkList<QAbstractButton *>::Iterator> queueMap;
};

#endif // IMPORTDIALOGPRIVATE_H
