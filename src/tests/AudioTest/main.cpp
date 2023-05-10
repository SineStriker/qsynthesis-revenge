#include "AudioTrack.h"
#include "Pcm32BitFloatAudioSource.h"
#include <QApplication>
#include <QDebug>
#include <QWindow>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavformat/avio.h"
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

//    if (argc < 2) {
//        qDebug().noquote() << QString("%1: <file>").arg(qAppName());
//        return 0;
//    }

//    QString fileName = QApplication::arguments().at(1);

    Pcm32BitFloatAudioSource src1("D:\\Downloads\\test_source1.pcm", 2, 48000);
    Pcm32BitFloatAudioSource src2("D:\\Downloads\\test_source2.pcm", 2, 48000);
    AudioTrack track(2, 48000);
    assert(track.addSource(&src1));
    track.addSource(&src2);
    track.open();
    QFile f1("D:\\Downloads\\test_output_c1.pcm");
    QFile f2("D:\\Downloads\\test_output_c2.pcm");
    f1.open(QIODevice::WriteOnly);
    f2.open(QIODevice::WriteOnly);
    AudioBufferList buf(2, 1024);
    for(int i = 0; i < 1048576 / 1024; i++) {
        buf.clearBuffer();
        track.read(buf, 1024);
        f1.write((char*)buf[0].data(), 1024 * sizeof(float));
        f2.write((char*)buf[1].data(), 1024 * sizeof(float));
    }
    auto testResampleFile = "D:\\Downloads\\test_resample.pcm";
    const char* argv1[2] = {argv[0], testResampleFile};
    return 0;
//    return main1(2, const_cast<char **>(argv1));
}