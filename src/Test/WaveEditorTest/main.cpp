#include "MainWindow.h"
#include <QApplication>
#ifdef Q_OS_WINDOWS
#include <windows.h>
#endif

int main(int argc, char *argv[]) {
    if (argc < 2) {
        qDebug() << "No audio file argument.";
        return 0;
    }

    QApplication a(argc, argv);

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
