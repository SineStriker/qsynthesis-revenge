#include "MainWindow.h"

#include "CApplication.h"

int main(int argc, char *argv[]) {
    CApplication a(argc, argv);

    QFont font("Microsoft YaHei");
    font.setStyleStrategy(QFont::PreferAntialias);
    a.setFont(font);

    MainWindow w;
    w.show();

    return a.exec();
}
