#include "DsInternalMain.h"

#include "Kernel/DsApplication.h"
#include "Kernel/DsStartupInfo.h"

#include "Managers/WindowManager.h"

#include <QMainWindow>

int main_entry(int argc, char *argv[]) {
    // Configure startup info
    DsStartupInfo info;
    
    // Qt Application
    DsApplication a(argc, argv);

    // Show window
    WindowManager::instance()->showHome();

    return a.exec();
}
