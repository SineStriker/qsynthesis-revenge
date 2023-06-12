#ifndef QMCONFIGURABLEDIALOG_H
#define QMCONFIGURABLEDIALOG_H

#include <QDialog>

#include "QMWidgetToolsGlobal.h"

class QMConfigurableDialogPrivate;

class QMWTOOLS_EXPORT QMConfigurableDialog : public QDialog {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QMConfigurableDialog)
public:
    explicit QMConfigurableDialog(QWidget *parent = nullptr);
    ~QMConfigurableDialog();

    void done(int r) override;

public:
    QWidget *widget() const;
    QWidget *takeWidget();
    void setWidget(QWidget *w);

    bool applyButtonVisible() const;
    void setApplyButtonVisible(bool visible);

protected:
    virtual void apply();
    virtual void finish();

signals:
    void applied();
    void finished();

protected:
    QMConfigurableDialog(QMConfigurableDialogPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<QMConfigurableDialogPrivate> d_ptr;
};


#endif // QMCONFIGURABLEDIALOG_H
