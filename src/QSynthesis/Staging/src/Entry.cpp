#include "qutastaging.h"

#include <QApplication>
#include <QMainWindow>

int main_entry(int argc, char *argv[]) {
    QApplication a(argc, argv);

    a.setApplicationName(APP_NAME);
    a.setApplicationDisplayName(APP_NAME);
    a.setApplicationVersion(APP_VERSION);

    QMainWindow w;
    w.show();

    return a.exec();
}
