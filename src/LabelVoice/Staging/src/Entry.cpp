#include "lvstaging.h"

#include "Kernel/LvApplication.h"
#include "Managers/QsPluginManager.h"
#include "Managers/WindowManager.h"

int main_entry(int argc, char *argv[]) {
    LvApplication a(argc, argv);

    WindowManager::instance()->showHome();

    return a.exec();
}
