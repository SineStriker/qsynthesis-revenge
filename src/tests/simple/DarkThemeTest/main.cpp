#include <QApplication>

#include "MainWindow.h"


int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

#ifdef Q_OS_WIN
    QFont font("Microsoft Yahei UI");
    font.setPointSize(9);
    a.setFont(font);
#endif

    MainWindow w;
    w.show();

    return a.exec();
}

