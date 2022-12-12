#include "gui/MainWindow.h"

#include "SystemHelper.h"

#include <QApplication>

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (Sys::isUserRoot()) {
        QString title = qAppName();
        QString msg = QString("You're trying to start %1 as the %2, which may cause "
                              "security problem and isn't recommended.")
                          .arg(qAppName(), "Administrator");
#ifdef Q_OS_WINDOWS
        ::MessageBoxW(0, msg.toStdWString().data(), title.toStdWString().data(),
                      MB_OK | MB_TOPMOST | MB_SETFOREGROUND | MB_ICONWARNING);
#elif Q_OS_LINUX
        fputs(qPrintable(msg), stdout);
#else
        QMessageBox::warning(nullptr, title, msg, CApplication::tr("Confirm"));
#endif
        return 0;
    }

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
