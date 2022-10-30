#include "MainWindow.h"

#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFont f("Microsoft YaHei");
    f.setPointSize(9);

    a.setFont(f);

    MainWindow w;
    w.show();

    return a.exec();
}
