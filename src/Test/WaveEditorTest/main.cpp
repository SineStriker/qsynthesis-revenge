
#include <QApplication>
#include "MainWindow.h"
#ifdef Q_OS_WINDOWS
    #include <windows.h>
#endif

int main(int argc, char *argv[])
{
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
    w.SetAudio("D:/tmp/song.mp3");
    w.show();

    return a.exec();
}