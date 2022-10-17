#include "QPmkFile.h"

#include <QDataStream>

QPmkFile::QPmkFile() {
}

QPmkFile::~QPmkFile() {
}

bool QPmkFile::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    QDataStream data(&file);
    char prefix[2];

    if (data.readRawData(prefix, 2) != 2 || prefix[0] != 0x13 || prefix[1] != 0x00) {
        file.close();
        return false;
    }

    double keyCycle;
    if (data.readRawData((char *) &keyCycle, 8) != 8) {
        file.close();
        return false;
    }

    double keyFrq;
    if (keyCycle < 50) {
        keyFrq = EMPTY_FREQUENCY;
    } else {
        keyFrq = 44100 / keyCycle;
    }

    KeyFrequency = keyFrq;

    int dataCount = 0;
    if (data.readRawData((char *) &dataCount, 4) != 4 || dataCount < 0) {
        file.close();
        return false;
    }

    if (dataCount == 0) {
        return true;
    }

    QVector<QPair<int, int>> vals;
    vals.resize(dataCount);
    for (int i = 0; i < dataCount; ++i) {
        int end, cycle;
        data.readRawData((char *) &end, 4);
        data.readRawData((char *) &cycle, 4);
        vals[i] = qMakePair(end, cycle);
    }
    file.close();

    int samplelength = vals.back().first + vals.back().second;
    int mapsize = (samplelength + 255) / 256;

    Frequency.clear();
    QVector<double> frqTable;
    frqTable.resize(mapsize);

    int offset = 0;
    int index = 0;
    for (int i = 0; i < mapsize; i++) {
        if (index > dataCount - 1)
            break;
        if (vals[index].second < 50) {
            frqTable[i] = EMPTY_FREQUENCY;
        } else {
            frqTable[i] = 44100.0 / vals[index].second;
        }
        offset += 256;
        while (index < dataCount && vals[index].first < offset) {
            index++;
        }
    }

    Frequency = frqTable.toList();
    return true;
}

bool QPmkFile::save(const QString &filename) {
    return true;
}
