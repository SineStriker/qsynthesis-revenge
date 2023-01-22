#include <QCoreApplication>
#include <QPluginLoader>

#include "Api/IAudioDecoder.h"
#include "QsMediaNamespace.h"

#include <chrono>
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    QPluginLoader loader;

    loader.setFileName("resources/modules/audiodecoders/FFmpegDecoder");
    auto decoder = qobject_cast<IAudioDecoder *>(loader.instance());

    assert(decoder);

    if (!decoder->open({{QsMedia::KEY_NAME_FILE_NAME, "D:/tmp/wave.wav"},
                        {QsMedia::KEY_NAME_SAMPLE_FORMAT, QsMedia::AV_SAMPLE_FMT_FLT}})) {
        return -1;
    }

    // Read arbitrary samples at a time
    constexpr int samplesToRead = 100;
    int samplesRead = 0;
    float *buffer = new float[samplesToRead];

    std::cerr << "Read " << samplesToRead << " samples at a time" << std::endl;

    auto start = std::chrono::high_resolution_clock::now();

    do {
        samplesRead = decoder->Read(buffer, 0, samplesToRead);
    } while (samplesRead > 0);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> diff = end - start;
    std::cerr << "Time taken: " << diff.count() << " s" << std::endl;

    loader.unload();

    return 0;
}
