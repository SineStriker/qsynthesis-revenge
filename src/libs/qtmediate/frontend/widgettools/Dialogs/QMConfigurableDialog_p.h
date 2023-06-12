#ifndef QMCONFIGURABLEDIALOG_P_H
#define QMCONFIGURABLEDIALOG_P_H

#include "QMConfigurableDialog.h"

#include <QVBoxLayout>

#include <CTabButton.h>
#include <QMEqualBoxLayout.h>

class QMConfigurableDialogPrivate : public QObject {
    Q_OBJECT
    Q_DECLARE_PUBLIC(QMConfigurableDialog)
public:
    QMConfigurableDialogPrivate();
    virtual ~QMConfigurableDialogPrivate();

    void init();

    void reloadStrings();

    QMConfigurableDialog *q_ptr;

    CTabButton *okButton, *cancelButton, *applyButton;
    QMEqualBoxLayout *buttonsLayout;

    QWidget *widget;

    QVBoxLayout *mainLayout;

    void apply_helper();
};


#endif // QMCONFIGURABLEDIALOG_P_H
