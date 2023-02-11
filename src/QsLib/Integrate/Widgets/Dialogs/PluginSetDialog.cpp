#include "PluginSetDialog.h"
#include "private/PluginSetDialog_p.h"

PluginSetDialog::PluginSetDialog(QWidget *parent)
    : PluginSetDialog(*new PluginSetDialogPrivate(), parent) {
}

PluginSetDialog::~PluginSetDialog() {
}

PluginSetDialog::PluginSetDialog(PluginSetDialogPrivate &d, QWidget *parent)
    : QDialog(parent), d_ptr(&d) {
    d.q_ptr = this;

    d.init();
}
