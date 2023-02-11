#include "PluginSetDialog_p.h"

#include "QsGuiPluginManager.h"
#include "private/QsGuiPluginManager_p.h"

class HackPluginManager : public QsGuiPluginManager {
public:
    friend class PluginSetDialogPrivate;
};

PluginSetDialogPrivate::PluginSetDialogPrivate() {
}

PluginSetDialogPrivate::~PluginSetDialogPrivate() {
}

void PluginSetDialogPrivate::init() {
}
