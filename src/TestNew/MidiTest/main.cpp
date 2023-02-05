#include <QCoreApplication>
#include <QDebug>
#include <QFile>

#include "com_global.h"

#include "QDspxModel.h"

MY_EXTERN_C_EXPORT int main_entry(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (argc < 2) {
        qDebug() << "Missing midi file name.";
        return -1;
    }

    QString midi = QString::fromLocal8Bit(argv[1]);
    if (!QFile::exists(midi)) {
        qDebug() << "File doesn't exist.";
        return -1;
    }

    qDebug().noquote() << QString("Midi: %1").arg(midi);

    QDspxModel dspx;
    bool res = QDspxModel::fromMidi(midi, &dspx);
    if (!res) {
        qDebug() << "Parse midi failed.";
        return -1;
    }

    qDebug() << "Parse midi successfully.";
    return 0;
}