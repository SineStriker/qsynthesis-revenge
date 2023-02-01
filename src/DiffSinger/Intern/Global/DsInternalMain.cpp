#include "DsInternalMain.h"

#include "Kernel/DsStartInfo.h"
#include "Managers/WindowManager.h"

#include "QsCss.h"

#include <QApplication>
#include <QDebug>
#include <QMainWindow>

int main_entry(int argc, char *argv[]) {
    // Qt Application
    QApplication a(argc, argv);

    // Configure startup info
    DsStartInfo info;
    info.parse();

    // Show window
    WindowManager::instance()->showHome();

    return a.exec();
}
