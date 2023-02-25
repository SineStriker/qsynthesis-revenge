#include <QApplication>
#include <QWidget>

#include "QMWidgetsHost.h"
#include "QsStartInfo.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    putenv("QTMEDIUM_BINARY_PATH=../bin");
    QMWidgetsHost host;
    QsStartInfo info;
    info.checkLoadInfo();

    QWidget w;
    w.show();

    return a.exec();
}
