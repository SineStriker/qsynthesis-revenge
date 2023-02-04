#include "QDspxModel.h"
#include "QMidiFile.h"

#include <QDebug>
#include <QFile>
#include <QTextCodec>

QDspxModel::QDspxModel() {
}

QDspxModel::~QDspxModel() {
}

bool QDspxModel::fromMidi(const QString &filename, QDspxModel *out) {
    QMidiFile midi;

    QMap<int, int> a;
    QHash<int, int> b;
    a[1] = 1;

    a[60] = 114514;

    // for (auto it = a.begin(); it != a.end(); ++it) {
    //     auto key = it.key();
    //     auto val = it.value();
    // }

    for (auto key : a.keys()) {
        auto val = a[key];
    }

    return false;
}
