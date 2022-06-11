#include "MainWindow.h"

#include "CApplication.h"

int main(int argc, char *argv[]) {
    CApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
