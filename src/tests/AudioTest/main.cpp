#include <QApplication>
#include <QDebug>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (argc < 2) {
        qDebug().noquote() << QString("%1: <file>").arg(qAppName());
        return 0;
    }

    QString fileName = QApplication::arguments().at(1);

    return a.exec();
}