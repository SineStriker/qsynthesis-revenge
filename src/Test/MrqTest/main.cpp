#include <QCoreApplication>

#include <QDebug>

#include "Frq/QMrqFile.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        qDebug() << "No mrq-file argument.";
        return 0;
    }

    QCoreApplication a(argc, argv);

    QMrqFile mrq;
    if (!mrq.load(argv[1])) {
        qDebug() << "Failed to load mrq file.";
    }

    qDebug() << "Version:" << mrq.version;
    for (auto it = mrq.entries.begin(); it != mrq.entries.end(); ++it) {
        qDebug() << int(it - mrq.entries.begin() + 1) << it->name << it->f0.size();
    }

    return a.exec();
}
