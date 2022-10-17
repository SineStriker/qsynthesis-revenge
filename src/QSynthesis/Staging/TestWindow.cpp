#include "TestWindow.h"

#include "FFmpegConverter.h"
#include "qsmedia_version.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>

TestWindow::TestWindow(QWidget *parent) : QMainWindow(parent) {
    text = new QLineEdit("zjsy.m4a", this);
    text->setGeometry(50, 50, 200, text->height());

    conv_btn = new QPushButton("convert", this);
    conv_btn->setGeometry(50, 100, 100, conv_btn->height());
    connect(conv_btn, &QPushButton::clicked, this, &TestWindow::_q_convButtonClicked);

    play_btn = new QPushButton("play", this);
    play_btn->setGeometry(50, 150, 100, conv_btn->height());
    connect(play_btn, &QPushButton::clicked, this, &TestWindow::_q_playButtonClicked);

    this->resize(500, 300);

    auto ffmpeg_info = FF::GetFFmpegInfo();
    for (const auto &info : ffmpeg_info) {
        qDebug() << info.name << ":"
                 << QString::asprintf("%d.%d.%d", info.version[0], info.version[1], info.version[2])
                 << info.license << info.configuration;
    }

    auto sdl_info = FF::GetSDLInfo();
    qDebug() << QString::asprintf("%d.%d.%d", sdl_info.version[0], sdl_info.version[1],
                                  sdl_info.version[2]);
}

TestWindow::~TestWindow() {
}

void TestWindow::_q_convButtonClicked() {
    FFmpegConverter conv;

    FFmpegConverter::PCMArgs args{FF::FMT_S16, 44100};
    FFmpegConverter::PCMResult res{};

    QFileInfo info(text->text());
    int err = conv.convert(info.absoluteFilePath(), args, &res);
    if (err < 0) {
        qDebug() << "Error convert." << err;
        return;
    }

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

        // uint8_t data[0]; // data pointer （不占内存）
    };

    WavHeader header{};

    auto wavHeader = &header;
    memcpy(&wavHeader->chunk_id, "RIFF", 4);
    wavHeader->chunk_size = 36 + res.bytes.size();
    memcpy(&wavHeader->format, "WAVE", 4);

    memcpy(&wavHeader->sub_chunk_1_id, "fmt ", 4);
    wavHeader->sub_chunk_1_size = 16;
    wavHeader->audio_format = 1;
    wavHeader->num_channels = res.channels;
    wavHeader->sample_rate = args.sampleRate;
    wavHeader->bit_per_sample = 16;
    wavHeader->block_align = wavHeader->num_channels * wavHeader->bit_per_sample / 8;
    wavHeader->byte_rate = wavHeader->sample_rate * wavHeader->block_align;

    memcpy(&wavHeader->sub_chunk_2_id, "data", 4);
    wavHeader->sub_chunk_2_size = res.bytes.size();

    QFile file(info.absolutePath() + "/" + info.baseName() + ".wav");
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Error create file." << err;
        return;
    }
    file.write((char *) wavHeader, sizeof(header));
    file.write(res.bytes.c_str(), res.bytes.size());
    file.close();

    qDebug() << "OK";
}

void TestWindow::_q_playButtonClicked() {
    //    SDL_version ver;
    //    SDL_GetVersion(&ver);
    //    qDebug() << ver.major << ver.minor << ver.patch;

    //    //设置显示application 的log信息
    //    SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    //    //设置sdl要初始化的模块
    //    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
    //        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "couldn't initialize SDL:%s\n",
    //        SDL_GetError()); return;
    //    }

    //    SDL_Log("available audio drivers");
    //    //打印音频设备
    //    for (int i = 0; i < SDL_GetNumAudioDrivers(); i++) {
    //        SDL_Log("%i:%s", i, SDL_GetAudioDriver(i));
    //    }

    //    //查看当前使用的音频设备
    //    SDL_Log("Using audio driver:%s\n", SDL_GetCurrentAudioDriver());

    //    qDebug() << "OK";
}
