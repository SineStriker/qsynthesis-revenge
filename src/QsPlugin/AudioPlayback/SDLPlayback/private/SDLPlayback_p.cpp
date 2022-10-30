#include "SDLPlayback_p.h"

#include <QDebug>

static const ushort PLAYBACK_SAMPLE_FORMAT = AUDIO_F32SYS; // 32位浮点

static const ushort PLAYBACK_BUFFER_SAMPLES = 1024; // 默认缓冲区长度

static const ushort PLAYBACK_SAMPLE_RATE = 44100; // 默认采样率

static const uint PLAYBACK_POLL_INTERVAL = 1; // 轮循时间间隔(ms)

static void workCallback(void *udata, quint8 *stream, int len) {
    auto d = (SDLPlaybackPrivate *) udata;
    d->workCallback(stream, len);
}

SDLPlaybackPrivate::SDLPlaybackPrivate() {
}

SDLPlaybackPrivate::~SDLPlaybackPrivate() {
}

void SDLPlaybackPrivate::init() {
    curDevId = 0;

    producer = nullptr;

    memset(&spec, 0, sizeof(spec));
}

bool SDLPlaybackPrivate::setup(const QVariantMap &args) {
    Q_Q(SDLPlayback);

    QSignalBlocker blocker(q);

    // Init SDL
    SDL_LogSetPriority((int) SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);

    // 初始化
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        qDebug().noquote() << QString("SDL: Failed to initialize SDL: %1").arg(SDL_GetError());
        return false;
    }

#ifdef Q_OS_WINDOWS
    if (!switchDriver("directsound")) {
        return false;
    }
#endif

    bufferSamples = (bufferSamples <= 0) ? PLAYBACK_BUFFER_SAMPLES : bufferSamples;
    sampleRate = (sampleRate <= 0) ? PLAYBACK_SAMPLE_RATE : sampleRate;

    // 配置音频参数结构体
    spec.freq = sampleRate;
    spec.format = PLAYBACK_SAMPLE_FORMAT;
    spec.channels = channels;
    spec.silence = 0;
    spec.samples = bufferSamples; //缓冲区字节数/单个采样字节数/声道数
    spec.callback = ::workCallback;
    spec.userdata = (void *) this; // 回调到成员函数

    // 缓冲区
    pcm_buffer_size = bufferSamples * channels;
    pcm_buffer.reset(new float[pcm_buffer_size * sizeof(float)]);

    Q_UNUSED(args);

    return true;
}

void SDLPlaybackPrivate::dispose() {
    Q_Q(SDLPlayback);

    QSignalBlocker blocker(q);

    switchDevId(0);
    switchDriver(QString());

    SDL_Quit();
}

void SDLPlaybackPrivate::play() {
    Q_Q(SDLPlayback);

    // 如果没有打开音频设备那么打开第一个音频设备
    if (curDevId == 0) {
        QStringList devices = q->devices();
        q->setDevice(devices.front());
    }

    // 初始化控制块
    scb.audio_chunk = nullptr;
    scb.audio_len = 0;
    scb.audio_pos = 0;

    // 启动生产者线程
    producer = new std::thread(&SDLPlaybackPrivate::poll, this);
}

void SDLPlaybackPrivate::stop() {
    // 结束生产者线程
    notifyStop();

    // 等待结束
    producer->join();

    delete producer;
    producer = nullptr;
}

void SDLPlaybackPrivate::switchState(IAudioPlayback::PlaybackState newState) {
    Q_Q(SDLPlayback);

    auto orgState = state.load();
    state = newState;

    // 通知播放状态已更改
    if (state != orgState) {
        emit q->stateChanged();
    }
}

bool SDLPlaybackPrivate::switchDriver(const QString &drv) {
    Q_Q(SDLPlayback);

    switchDevId(0);

    if (!driver.isEmpty()) {
        SDL_AudioQuit();
    }
    if (!drv.isEmpty()) {
        if (SDL_AudioInit(drv.toStdString().data()) < 0) {
            qDebug().noquote().noquote()
                << QString("SDLHost: Failed to initialize SDL: %1.").arg(SDL_GetError());
            return false;
        }
    }

    auto orgDriver = driver;
    driver = drv;

    if (driver != orgDriver) {
        emit q->driverChanged();
    }

    return true;
}

void SDLPlaybackPrivate::switchDevId(int newId) {
    Q_Q(SDLPlayback);

    auto orgId = curDevId;
    curDevId = newId;

    if (orgId > 0) {
        SDL_CloseAudioDevice(orgId);
    }

    // 通知音频设备已更改
    if (curDevId != orgId) {
        emit q->deviceChanged();
    }
}

void SDLPlaybackPrivate::notifyGetAudioFrame() {
    SDL_Event e;
    e.type = (SDL_EventType) SDL_EVENT_BUFFER_END;
    SDL_PushEvent(&e);
}

void SDLPlaybackPrivate::notifyStop() {
    SDL_Event e;
    e.type = (SDL_EventType) SDL_EVENT_MANUAL_STOP;
    SDL_PushEvent(&e);
}

void SDLPlaybackPrivate::workCallback(quint8 *stream, int len) {
    // 上锁
    mutex.lock();

    // 缓冲区置为静音
    SDL_memset(stream, 0, len);

    if (scb.audio_len > 0 && scb.audio_chunk) {
        len = qMin(len, scb.audio_len);

        // 将缓冲区中的声音写入流
        SDL_MixAudioFormat(stream, (quint8 *) (scb.audio_chunk + scb.audio_pos), spec.format,
                           (uint) len, SDL_MIX_MAXVOLUME);

        scb.audio_pos += len;
        scb.audio_len -= len;

        // 判断是否完毕
        if (scb.audio_len == 0) {
            notifyGetAudioFrame();
        }
    }

    // 放锁
    mutex.unlock();
}

void SDLPlaybackPrivate::poll() {

    // 设置暂停标识位
    SDL_PauseAudioDevice(curDevId, 0);
    switchState(IAudioPlayback::Playing);

    // 第一次事件
    notifyGetAudioFrame();

    // 外层循环
    while (true) {
        bool over = false;

        // 不停地获取事件
        SDL_Event e;
        while (SDL_PollEvent(&e) > 0) {
            switch ((int) e.type) {
                // 缓存用完
                case SDL_EVENT_BUFFER_END: {
                    // 上锁
                    mutex.lock();

                    float *pcm_buffer = this->pcm_buffer.data();

                    // 从文件中读取数据，剩下的就交给音频设备去完成了
                    // 它播放完一段数据后会执行回调函数，获取等多的数据
                    int samples = decoder->Read(pcm_buffer, 0, spec.samples * spec.channels);
                    if (samples <= 0) {
                        // 播放完毕
                        over = true;
                    } else {
                        // 重置缓冲区
                        scb.audio_chunk = (char *) pcm_buffer;
                        scb.audio_len =
                            samples * 4; // 长度为读出数据长度，在read_audio_data中做减法
                        scb.audio_pos = 0; // 设置当前位置为缓冲区头部
                    }

                    // 放锁
                    mutex.unlock();
                    break;
                }

                // 停止命令
                case SDL_EVENT_MANUAL_STOP: {
                    over = true;
                    break;
                }

                // 音频设备移除
                case SDL_AUDIODEVICEREMOVED: {
                    quint32 dev = e.adevice.which;
                    if (dev == curDevId) {
                        switchDevId(0);
                        over = true;
                    }

                    break;
                }
                default:
                    break;
            }
        }

        if (over) {
            break;
        }

        SDL_Delay(PLAYBACK_POLL_INTERVAL);
    }

    // 设置暂停标识位
    SDL_PauseAudioDevice(curDevId, 1);
    switchState(IAudioPlayback::Stopped);
}
