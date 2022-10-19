#include "Kernel/QsApplication.h"
#include "Managers/PluginManager.h"
#include "Managers/WindowManager.h"

int main(int argc, char *argv[]) {
    QsApplication a(argc, argv);

    WindowManager::instance()->showHome();

    return a.exec();
}
