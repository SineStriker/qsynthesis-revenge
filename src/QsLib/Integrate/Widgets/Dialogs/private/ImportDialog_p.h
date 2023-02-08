#ifndef IMPORTDIALOGPRIVATE_H
#define IMPORTDIALOGPRIVATE_H

#include "../ImportDialog.h"

#include "CTabButton.h"
#include "Frames/LinearScrollArea.h"

#include <QAction>
#include <QButtonGroup>
#include <QLabel>
#include <QTableWidget>
#include <QVBoxLayout>

#include <list>

class ImportDialogPrivate : public QObject {
    Q_OBJECT
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
    QTabWidget *tabWidget;

    QVBoxLayout *layout;
    QVBoxLayout *boxesLayout;
    QHBoxLayout *buttonsLayout;

    QWidget *boxesWidget;
    QScrollArea *boxesScroll;

    QLabel *lbCaption;
    CTabButton *btnCancel, *btnOK;

    QAction *okAction;
    QButtonGroup *boxGroup;

    std::list<QAbstractButton *> queue;
    QHash<QAbstractButton *, std::list<QAbstractButton *>::iterator> queueMap;

    bool firstShow;
    int maxInitHeight;

private:
    void _q_boxToggled(bool checked);
    void _q_okButtonClicked();
    void _q_cancelButtonClicked();
    void _q_scrollRangeChanged(int min, int max);
};


#endif // IMPORTDIALOGPRIVATE_H
