#include "MainWindow.h"

#include <QApplication>

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QFont f("Microsoft YaHei");
    f.setPointSize(9);
    a.setFont(f);

    // Set library loading info
#ifdef Q_OS_WINDOWS
    ::SetDllDirectoryW(
        QString(qApp->applicationDirPath() + "/" + TEST_LIB_DIR).toStdWString().c_str());
#endif
    qApp->addLibraryPath(qApp->applicationDirPath() + "/" + TEST_APP_DIR + "/plugins");
    qApp->addLibraryPath(qApp->applicationDirPath() + "/" + TEST_APP_DIR + "/resources/modules");
    qApp->addLibraryPath(qApp->applicationDirPath() + "/" + TEST_APP_DIR + "/resources/plugins");

    MainWindow w;
    w.show();

    return a.exec();
}
