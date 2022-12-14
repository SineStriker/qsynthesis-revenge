#include "qutastaging.h"

#include "Kernel/QUtaApplication.h"
#include "Kernel/QUtaStartupInfo.h"

#include <QMainWindow>

int main_entry(int argc, char *argv[]) {
    // Configure startup info
    QUtaStartupInfo info;
    
    // Qt Application
    QUtaApplication a(argc, argv);

    QMainWindow w;
    w.show();

    return a.exec();
}
