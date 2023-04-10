#ifndef CONFIGURABLEDIALOGPRIVATE_H
#define CONFIGURABLEDIALOGPRIVATE_H

#include "../ConfigurableDialog.h"

#include <QVBoxLayout>

#include "CTabButton.h"
#include "QMEqualBoxLayout.h"

namespace QsApi {

    class ConfigurableDialogPrivate : public QObject {
        Q_OBJECT
        Q_DECLARE_PUBLIC(ConfigurableDialog)
    public:
        ConfigurableDialogPrivate();
        virtual ~ConfigurableDialogPrivate();

        void init();

        void reloadStrings();

        ConfigurableDialog *q_ptr;

        CTabButton *okButton, *cancelButton, *applyButton;
        QMEqualBoxLayout *buttonsLayout;

        QWidget *widget;

        QVBoxLayout *mainLayout;

        void apply_helper();
    };

}

#endif // CONFIGURABLEDIALOGPRIVATE_H