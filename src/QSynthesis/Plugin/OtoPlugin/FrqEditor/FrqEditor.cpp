#include "FrqEditor.h"

FrqEditor::FrqEditor(QObject *parent) : IOtoPlugin(parent) {
}

FrqEditor::~FrqEditor() {
}

int FrqEditor::exec(const IOtoPlugin::PluginArguments &args, void *userdata) {
    return 0;
}
