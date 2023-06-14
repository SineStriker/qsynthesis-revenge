#ifndef CCONFIGURABLEDIALOG_H
#define CCONFIGURABLEDIALOG_H

#include <QDialog>

#include "QMWidgetToolsGlobal.h"

class CConfigurableDialogPrivate;

class QMWTOOLS_EXPORT CConfigurableDialog : public QDialog {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CConfigurableDialog)
public:
    explicit CConfigurableDialog(QWidget *parent = nullptr);
    ~CConfigurableDialog();

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
    CConfigurableDialog(CConfigurableDialogPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<CConfigurableDialogPrivate> d_ptr;
};


#endif // CCONFIGURABLEDIALOG_H
