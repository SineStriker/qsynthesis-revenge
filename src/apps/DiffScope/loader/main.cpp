#include <QApplication>
#include <QDebug>
#include <QMainWindow>

#include "QMWidgetsHost.h"
#include "QsStartInfo.h"

extern "C" Q_DECL_EXPORT int main_entry(int argc, char *argv[]);

int main_entry(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    QFont f("Microsoft YaHei UI");
    f.setPixelSize(15);
    a.setFont(f);

    QMWidgetsHost host;
    QsStartInfo info;
    info.checkLoadInfo();

    QMainWindow w;
    w.show();

    return a.exec();
}
