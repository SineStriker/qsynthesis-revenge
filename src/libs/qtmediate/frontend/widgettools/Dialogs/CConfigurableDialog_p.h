#ifndef CCONFIGURABLEDIALOG_P_H
#define CCONFIGURABLEDIALOG_P_H

#include "CConfigurableDialog.h"

#include <QVBoxLayout>

#include <CTabButton.h>
#include <QMEqualBoxLayout.h>

class CConfigurableDialogPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(CConfigurableDialog)
public:
    CConfigurableDialogPrivate();
    virtual ~CConfigurableDialogPrivate();

    void init();

    void reloadStrings();

    CConfigurableDialog *q_ptr;

    CTabButton *okButton, *cancelButton, *applyButton;
    QMEqualBoxLayout *buttonsLayout;

    QWidget *widget;

    QVBoxLayout *mainLayout;

    void apply_helper();
};


#endif // CCONFIGURABLEDIALOG_P_H
