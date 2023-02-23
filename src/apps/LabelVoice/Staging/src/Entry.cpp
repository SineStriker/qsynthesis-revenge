#include "lvstaging.h"

#include "Kernel/LvStartupInfo.h"
#include "Kernel/LvApplication.h"

#include "Managers/QsPluginManager.h"
#include "Managers/WindowManager.h"

int main_entry(int argc, char *argv[]) {
    // Configure startup info
    LvStartupInfo info;

    // Qt Application
    LvApplication a(argc, argv);

    // Show window
    WindowManager::instance()->showHome();

    return a.exec();
}
