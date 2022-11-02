#include "Kernel/LvApplication.h"
#include "Managers/PluginManager.h"
#include "Managers/WindowManager.h"

#include "Entry.h"

int main_entry(int argc, char *argv[]) {
    LvApplication a(argc, argv);

    WindowManager::instance()->showHome();

    return a.exec();
}
