#include "MainWindow.h"
#include "QsApplication.h"

#include "Test/TestWindow.h"

int main(int argc, char *argv[]) {
    QsApplication a(argc, argv);

    TestWindow w;
    w.show();

    return a.exec();
}
