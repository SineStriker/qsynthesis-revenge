#include <utility>
#include <vector>
#include <tuple>
#include <string>
#include <filesystem>
#include <sstream>
#include <sndfile.hh>

#include <QFile>
#include <QFileInfo>
#include <QDir>


#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32)) || (defined(UNICODE) || defined(_UNICODE))
#define USE_WIDE_CHAR
#endif

#include "workthread.h"
#include "slicer.h"

inline int determineSndFileFormat(int formatEnum);

WorkThread::WorkThread(
        QString filename,
        QString outPath,
        double threshold,
        qint64 minLength,
        qint64 minInterval,
        qint64 hopSize,
        qint64 maxSilKept,
        int outputWaveFormat
    )
        : m_filename(std::move(filename)),
          m_outPath(std::move(outPath)),
          m_threshold(threshold),
          m_minLength(minLength),
          m_minInterval(minInterval),
          m_hopSize(hopSize),
          m_maxSilKept(maxSilKept),
          m_outputWaveFormat(outputWaveFormat),
          m_decoder(nullptr),
          m_decoderPlugin(nullptr)
{}

void WorkThread::run() {
    try {
        emit oneInfo(QString("%1 started processing.").arg(m_filename));

#ifdef USE_WIDE_CHAR
        std::wstring filename = m_filename.toStdWString();

        auto path = std::filesystem::absolute(filename);
        auto out = m_outPath.isEmpty() ? path.parent_path() : std::filesystem::path(m_outPath.toStdWString());
#else
        std::string filename = m_filename.toStdString();

        auto path = std::filesystem::absolute(filename);
        auto out = m_outPath.isEmpty() ? path.parent_path() : std::filesystem::path(m_outPath.toStdString());
#endif
        auto pathh = QDir(m_filename).absolutePath();
        auto dirname = QFileInfo(m_filename).absolutePath();
        initPlugins();
        QsMedia::WaveArguments wa;
        wa.sampleFormat = QsMedia::AV_SAMPLE_FMT_FLT;
        auto succeed = m_decoder->open(m_filename, wa);
        if (!succeed) {
            emit oneError(QString("Error opening file! ") + m_filename);
            return;
        }

        auto wavFmt = m_decoder->Format();
        int channels = wavFmt.Channels();
        int sr = wavFmt.SampleRate();
        int bits = wavFmt.BitsPerSample();
        auto frames = m_decoder->TotalSamples();

        auto total_size = frames * channels;

        Slicer slicer(m_decoder, m_threshold, m_minLength, m_minInterval, m_hopSize, m_maxSilKept);
        auto chunks = slicer.slice();

        if (chunks.empty()) {
            QString errmsg = QString("ValueError: The following conditions must be satisfied: (m_minLength >= m_minInterval >= m_hopSize) and (m_maxSilKept >= m_hopSize).");
            emit oneError(errmsg);
            return;
        }

        if (!std::filesystem::exists(out)) {
            std::filesystem::create_directories(out);
        }

        bool write_error = false;
        int idx = 0;
        for (auto chunk : chunks) {
            auto begin_frame = std::get<0>(chunk);
            auto end_frame = std::get<1>(chunk);
            auto frame_count = (qint64)(end_frame - begin_frame);
            if ((begin_frame == end_frame) || (begin_frame > total_size) || (end_frame > total_size)) {
                continue;
            }
            qDebug() << "begin frame: " << begin_frame << " (" << 1.0 * begin_frame / sr << " seconds) " << '\n' << "end frame: " << end_frame << " (" << 1.0 * end_frame / sr << " seconds) "  << '\n';
#ifdef USE_WIDE_CHAR
            std::wstringstream ss;
            ss << std::filesystem::path(filename).stem().wstring() << L"_" << idx << L".wav";
            std::filesystem::path out_file_path = out / ss.str();
            std::wstring out_file_path_str = out_file_path.wstring();
#else
            std::stringstream ss;
            ss << std::filesystem::path(filename).stem().string() << "_" << idx << ".wav";
            std::filesystem::path out_file_path = out / ss.str();
            std::string out_file_path_str = out_file_path.string();
#endif
            int sndfile_outputWaveFormat = determineSndFileFormat(m_outputWaveFormat);
            SndfileHandle wf = SndfileHandle(out_file_path_str.c_str(), SFM_WRITE,
                                             SF_FORMAT_WAV | sndfile_outputWaveFormat, channels, sr);
            m_decoder->SetCurrentSample(0);
            m_decoder->SkipSamples(begin_frame * channels);
            std::vector<float> tmp((end_frame - begin_frame) * channels);
            m_decoder->Read(tmp.data(), 0, tmp.size());
            auto bytes_written = wf.write(tmp.data(), tmp.size());
            if (bytes_written == 0) {
                write_error = true;
            }
            idx++;
        }
        if (write_error) {
            QString errmsg = QString("Zero bytes written");
            emit oneError(errmsg);
            return;
        }
    }
    catch (const std::filesystem::filesystem_error& err) {
        QString errmsg = QString("Filesystem error: %1").arg(err.what());
        emit oneError(errmsg);
        return;
    }

    emit oneFinished(m_filename);
}

void WorkThread::initPlugins() {
#ifdef Q_OS_LINUX
    m_decoderLoader.setFileName("audiodecoders/libFFmpegDecoder");
#else
    m_decoderLoader.setFileName("audiodecoders/FFmpegDecoder");
#endif

    m_decoder = nullptr;

    m_decoderPlugin = qobject_cast<QsApi::IAudioDecoderPlugin *>(m_decoderLoader.instance());

    if (!m_decoderPlugin) {
        //QMessageBox::critical(
        //    this, qApp->applicationName(),
        //    QString("Failed to load plugins: %1!").arg(decoderLoader.errorString()));
        emit oneError(QString("Failed to load plugins: %1!").arg(m_decoderLoader.errorString()));
        goto out;
    }

    m_decoder = m_decoderPlugin->create({}, this);
    if (!m_decoder) {
        //QMessageBox::critical(this, qApp->applicationName(), QString("Failed to create decoder object!"));
        emit oneError(QString("Failed to create decoder object!"));
        goto out2;
    }

    return;

out2:
    // Must delete before uninitializing the plugins.
    // If rely on the destructor of QWidget we would be calling unloaded code upon exit and crash the program
    delete m_decoder;

out:
    uninitPlugins();
}

void WorkThread::uninitPlugins() {
    delete m_decoder;
    m_decoderLoader.unload();
}

inline int determineSndFileFormat(int formatEnum) {
    switch (formatEnum) {
        case WF_INT16_PCM:
            return SF_FORMAT_PCM_16;
        case WF_INT24_PCM:
            return SF_FORMAT_PCM_24;
        case WF_INT32_PCM:
            return SF_FORMAT_PCM_32;
        case WF_FLOAT32:
            return SF_FORMAT_FLOAT;
    }
    return 0;
}