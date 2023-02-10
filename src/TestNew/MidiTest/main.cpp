#include <QApplication>
#include <QDebug>
#include <QFile>

#include "com_global.h"

#include "QsStartInfo.h"

#include <QLabel>
#include <QMainWindow>

COM_EXTERN_C_EXPORT int main_entry(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QFont f("Microsoft YaHei UI");
    f.setPixelSize(15);
    a.setFont(f);

    QsStartInfo info;
    info.load();

    if (argc < 2) {
        qDebug() << "Missing midi file name.";
        return -1;
    }

    QMainWindow w;
    w.setCentralWidget(new QLabel("Hello World!"));
    w.show();

    QString midi = QString::fromLocal8Bit(argv[1]);
    if (!QFile::exists(midi)) {
        qDebug() << "File doesn't exist.";
        return -1;
    }

    qDebug().noquote() << QString("Midi: %1").arg(midi);

    QDspxModel dspx;
    bool res = Import::loadMidi(midi, &dspx);
    if (!res) {
        qDebug() << "Parse midi failed.";
        return -1;
    }

    qDebug() << "Parse midi successfully.";

    return a.exec();
}
