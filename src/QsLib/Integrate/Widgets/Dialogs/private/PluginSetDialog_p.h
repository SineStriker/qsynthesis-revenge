#ifndef PLUGINSETDIALOGPRIVATE_H
#define PLUGINSETDIALOGPRIVATE_H

#include "../PluginSetDialog.h"

class PluginSetDialogPrivate {
    Q_DECLARE_PUBLIC(PluginSetDialog)
public:
    PluginSetDialogPrivate();
    virtual ~PluginSetDialogPrivate();

    void init();

    PluginSetDialog *q_ptr;
};

#endif // PLUGINSETDIALOGPRIVATE_H
