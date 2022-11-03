#include "spek-spectrogram.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("No audio argument.\n");
        return 0;
    }

    QApplication a(argc, argv);

    SpekSpectrogram w;
    w.show();

    w.open(argv[1]);

    return a.exec();
}
