#include <QApplication>
#include <QDebug>
#include <QMainWindow>

extern "C" Q_DECL_EXPORT int main_entry(int argc, char *argv[]);

int main_entry(int argc, char *argv[]) {
    if (qgetenv("CHORUSKIT_EXTERN_WINMAIN") == "1") {
        qputenv("QTMEDIUM_BINARY_PATH", "bin");
    }

    QApplication a(argc, argv);
    QFont f("Microsoft YaHei UI");
    f.setPixelSize(15);
    a.setFont(f);

    QMainWindow w;
    w.show();

    return a.exec();
}