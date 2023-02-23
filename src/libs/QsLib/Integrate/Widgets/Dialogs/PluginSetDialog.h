#ifndef PLUGINSETDIALOG_H
#define PLUGINSETDIALOG_H

#include <QDialog>

#include "QsIntegrateGlobal.h"

class PluginSetDialogPrivate;

class QSINTEGRATE_API PluginSetDialog : public QDialog {
    Q_OBJECT
    Q_DECLARE_PRIVATE(PluginSetDialog)
public:
    explicit PluginSetDialog(QWidget *parent = nullptr);
    ~PluginSetDialog();

protected:
    PluginSetDialog(PluginSetDialogPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<PluginSetDialogPrivate> d_ptr;
};

#endif // PLUGINSETDIALOG_H
