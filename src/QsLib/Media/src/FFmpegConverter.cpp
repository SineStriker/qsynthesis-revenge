#include "FFmpegConverter.h"

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/avutil.h"
#include "libswresample/swresample.h"
}

#include <QDebug>

FFmpegConverter::FFmpegConverter() {
}

FFmpegConverter::~FFmpegConverter() {
}

int FFmpegConverter::convert(const QString &filename, const FFmpegConverter::PCMArgs &args,
                             FFmpegConverter::PCMResult *pcm_pack) {

    AVFormatContext *fmt_ctx = avformat_alloc_context();

    AVCodecParameters *codec_param;
    const AVCodec *codec;
    AVCodecContext *codec_ctx;

    AVPacket *pkt;
    AVFrame *frame;
    SwrContext *swr;

    int audio_idx;

    AVChannelLayout in_ch_layout;
    AVChannelLayout out_ch_layout;

    int out_sample_rate = args.sampleRate;
    auto out_sample_fmt = static_cast<AVSampleFormat>(args.fmt);

    int out_channels;
    uint8_t *out_buf;

    //    AVAudioFifo *fifo;

    // 打开文件
    int ans = avformat_open_input(&fmt_ctx, filename.toStdString().c_str(), nullptr, nullptr);
    if (ans) {
        qWarning() << "Failing to open file!";

        ans = -1;
        goto out_fmt;
    }

    // 查找流信息
    ans = avformat_find_stream_info(fmt_ctx, nullptr);
    if (ans < 0) {
        qWarning() << "Failing to find stream info!";

        ans = -1;
        goto out_fmt;
    }

    // 输出基本信息
    qDebug() << "Info:";
    {
        qDebug() << "Duration:" << fmt_ctx->duration;
        qDebug() << "Streams:" << fmt_ctx->nb_streams;

        for (uint32_t i = 0; i < fmt_ctx->nb_streams; i++) {
            qDebug() << QString::asprintf(
                "    %d. %s", i,
                av_get_media_type_string(fmt_ctx->streams[i]->codecpar->codec_type));
        }

        qDebug() << QString::asprintf("  Iformat name: %s", fmt_ctx->iformat->name);
        qDebug() << QString::asprintf("  Iformat long name: %s", fmt_ctx->iformat->long_name);

        fflush(stdout);
    }

    // 查找音频流
    audio_idx = av_find_best_stream(fmt_ctx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (audio_idx < 0) {
        qWarning() << "Failing to find audio stream!";

        ans = -1;
        goto out_fmt;
    }

    // 查找解码器
    codec_param = fmt_ctx->streams[audio_idx]->codecpar;
    codec = avcodec_find_decoder(codec_param->codec_id);
    if (!codec) {
        qWarning() << "Failing to find decoder!";

        ans = -1;
        goto out_fmt;
    }

    // 分配解码器上下文
    codec_ctx = avcodec_alloc_context3(nullptr);
    if (!codec_ctx) {
        qWarning() << "Failing to alloc context!";

        ans = -1;
        goto out_fmt;
    }

    // 传递解码器信息
    ans = avcodec_parameters_to_context(codec_ctx, codec_param);
    if (ans < 0) {
        qWarning() << "Failing to pass params to context!";
        goto out_fmt;
    }

    // 打开解码器
    ans = avcodec_open2(codec_ctx, codec, nullptr);
    if (ans < 0) {
        qWarning() << "Failing to open codec!";
        goto out_fmt;
    }

    in_ch_layout = codec_ctx->ch_layout;
    out_ch_layout = in_ch_layout;

    // 输出源与目标信息
    {
        qDebug() << "Source:";
        qDebug() << QString::asprintf("  Sample format: %s",
                                      av_get_sample_fmt_name(codec_ctx->sample_fmt));
        qDebug() << QString::asprintf("  Sample rate: %d", codec_ctx->sample_rate);
        qDebug() << QString::asprintf("  Sample channels: %d", in_ch_layout.nb_channels);

        printf("Dest:\n");
        qDebug() << QString::asprintf("  Sample format: %s",
                                      av_get_sample_fmt_name(out_sample_fmt));
        qDebug() << QString::asprintf("  Sample rate: %d", out_sample_rate);
        qDebug() << QString::asprintf("  Sample channels: %d", out_ch_layout.nb_channels);
    }

    // 初始化重采样器
    swr = swr_alloc();
    ans = swr_alloc_set_opts2(&swr, &out_ch_layout, out_sample_fmt, out_sample_rate, &in_ch_layout,
                              codec_ctx->sample_fmt, codec_ctx->sample_rate, 0, nullptr);
    if (ans != 0) {
        qWarning() << "Failing to allocate swr set!";
        goto out_swr;
    }

    if (swr_init(swr) < 0) {
        qWarning() << "Failing to init swr!";
        goto out_swr;
    }

    out_channels = out_ch_layout.nb_channels;
    out_buf = (uint8_t *) av_malloc(2 * 44100);

    pcm_pack->channels = out_channels;
    pcm_pack->bytes.clear();

    pkt = av_packet_alloc();
    frame = av_frame_alloc();

    while (true) {
        int err = av_read_frame(fmt_ctx, pkt);
        // 判断是否结束
        if (err != 0) {
            av_packet_unref(pkt);
            break;
        }

        // 跳过其他流
        if (pkt->stream_index != audio_idx) {
            av_packet_unref(pkt);
            continue;
        }

        // 发送待解码包
        int res = avcodec_send_packet(codec_ctx, pkt);
        av_packet_unref(pkt);
        if (res < 0) {
            qWarning() << "Error submitting a packet for decoding.";

            ans = err;
            break;
        }

        while (res >= 0) {
            // 接收解码数据
            res = avcodec_receive_frame(codec_ctx, frame);
            if (res == AVERROR_EOF || res == AVERROR(EAGAIN)) {
                // 结束
                break;
            } else if (res < 0) {
                // 出错
                qWarning() << "Error decoding frame.";
                av_frame_unref(frame);
                break;
            }

            // 进行重采样
            AVFrame *resampled_frame = av_frame_alloc();
            resampled_frame->sample_rate = out_sample_rate;
            resampled_frame->ch_layout = out_ch_layout;
            resampled_frame->format = out_sample_fmt;

            err = swr_convert_frame(swr, resampled_frame, frame);
            if (err == 0) {
                int sz = av_samples_get_buffer_size(nullptr, resampled_frame->ch_layout.nb_channels,
                                                    resampled_frame->nb_samples, out_sample_fmt, 1);
                pcm_pack->bytes.append(std::string((char *) resampled_frame->data[0], sz));
            }

            av_frame_free(&resampled_frame);
            av_frame_unref(frame);
        }
    }

    av_frame_free(&frame);
    av_free(out_buf);

out_swr:
    swr_free(&swr);

out_codec:
    avcodec_close(codec_ctx);

out_fmt:
    avformat_close_input(&fmt_ctx);

    fflush(stdout);
    return ans;
}
