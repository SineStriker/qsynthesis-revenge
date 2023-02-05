#include "DsHostMain.h"

#include "Kernel/DsApplication.h"
#include "Managers/WindowManager.h"

#include <QDebug>

int main_entry(int argc, char *argv[]) {
    // Init application and startInfo
    DsApplication a(argc, argv);

    // Show window
    WindowManager::instance()->showHome();

    return a.exec();
}
