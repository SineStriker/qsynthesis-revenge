
#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.SetAudio("D:/tmp/song.wav");
    w.show();

    return a.exec();
}