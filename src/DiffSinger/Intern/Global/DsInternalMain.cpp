#include "DsInternalMain.h"

#include "Kernel/DsApplication.h"
#include "Kernel/DsStartInfo.h"

#include "Managers/WindowManager.h"

#include <QMainWindow>

int main_entry(int argc, char *argv[]) {
    // Configure startup info
    DsStartInfo info;
    
    // Qt Application
    DsApplication a(argc, argv);

    // Show window
    WindowManager::instance()->showHome();

    return a.exec();
}
