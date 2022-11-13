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
    ::SetDllDirectoryW(QString(qApp->applicationDirPath() + "/lib").toStdWString().c_str());
#endif
    qApp->addLibraryPath(qApp->applicationDirPath() + "/plugins");
    qApp->addLibraryPath(qApp->applicationDirPath() + "/resources/modules");
    qApp->addLibraryPath(qApp->applicationDirPath() + "/resources/plugins");

    MainWindow w;
    w.show();

    return a.exec();
}
