#include "WaveObject.h"

#include <QAudioOutput>
#include <QDebug>

struct WavHeader {
    uint32_t chunk_id;   // "RIFF"
    uint32_t chunk_size; // (44-8) + pcm_data_len
    uint32_t format;     // "WAVE"

    uint32_t sub_chunk_1_id;   // "fmt "
    uint32_t sub_chunk_1_size; // 16 (PCM)
    uint16_t audio_format;     //  1 - PCM
    uint16_t num_channels;     // 1-mono, 2-stero
    uint32_t sample_rate;
    uint32_t byte_rate;   // sample_rate * block_align
    uint16_t block_align; // num_channels * bit_per_sample
    uint16_t bit_per_sample;

    uint32_t sub_chunk_2_id;   // "data"
    uint32_t sub_chunk_2_size; //  pcm_data_len
};

WaveObject::WaveObject(QObject *parent) : QObject(parent) {
    dev = nullptr;
}

WaveObject::~WaveObject() {
}

void WaveObject::start() {
    qDebug() << "WaveObject: Start";

    exitFlag = 0;

    // Write filename
    dev->write(fileName.toUtf8());
    dev->write("\n");

    int cnt;
    QByteArray buffer;
    while (true) {
        // Polling

        if ((cnt = dev->bytesAvailable()) > 0) {
            buffer.append(dev->read(cnt));
            qDebug() << buffer.size();
        }

        if (exitFlag != 0) {
            break;
        }
    }

    qDebug() << "WaveObject: Finish";

    emit finished();
}
