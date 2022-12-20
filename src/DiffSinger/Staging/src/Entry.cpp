#include "dsstaging.h"

#include "Kernel/DsApplication.h"
#include "Kernel/DsStartupInfo.h"

#include <QMainWindow>

int main_entry(int argc, char *argv[]) {
    // Configure startup info
    DsStartupInfo info;
    
    // Qt Application
    DsApplication a(argc, argv);

    QMainWindow w;
    w.show();

    return a.exec();
}
