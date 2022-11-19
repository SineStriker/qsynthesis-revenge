#include "qustaging.h"

#include <QApplication>
#include <QMainWindow>

int main_entry(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QMainWindow w;
    w.show();

    return a.exec();
}
