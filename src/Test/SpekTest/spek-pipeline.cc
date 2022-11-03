#include <assert.h>
#include <qmath.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

#include "spek-audio.h"
#include "spek-fft.h"
#include "spek-pipeline.h"

#include <QDebug>
#include <QObject>

#include <condition_variable>
#include <mutex>
#include <thread>

enum {
    NFFT = 64 // Number of FFTs to pre-fetch.
};

struct spek_pipeline {
    std::unique_ptr<AudioFile> file;
    std::unique_ptr<FFTPlan> fft;
    int stream;
    int channel;
    enum window_function window_function;
    int samples;
    spek_pipeline_cb cb;
    void *cb_data;

    float *coss; // Pre-computed cos table.
    int nfft;    // Size of the FFT transform.
    int input_size;
    int input_pos;
    float *input;
    float *output;

    std::thread reader_thread;
    bool has_reader_thread;
    std::mutex reader_mutex;
    bool has_reader_mutex;
    std::condition_variable reader_cond;
    bool has_reader_cond;
    std::thread worker_thread;
    bool has_worker_thread;
    std::mutex worker_mutex;
    bool has_worker_mutex;
    std::condition_variable worker_cond;
    bool has_worker_cond;
    bool worker_done;
    volatile bool quit;
};

// Forward declarations.
static void *reader_func(void *);
static void *worker_func(void *);
static void reader_sync(struct spek_pipeline *p, int pos);

struct spek_pipeline *spek_pipeline_open(std::unique_ptr<AudioFile> file,
                                         std::unique_ptr<FFTPlan> fft, int stream, int channel,
                                         enum window_function window_function, int samples,
                                         spek_pipeline_cb cb, void *cb_data) {
    spek_pipeline *p = new spek_pipeline();
    p->file = std::move(file);
    p->fft = std::move(fft);
    p->stream = stream;
    p->channel = channel;
    p->window_function = window_function;
    p->samples = samples;
    p->cb = cb;
    p->cb_data = cb_data;

    p->coss = nullptr;
    p->input = nullptr;
    p->output = nullptr;
    p->has_reader_thread = false;
    p->has_reader_mutex = false;
    p->has_reader_cond = false;
    p->has_worker_thread = false;
    p->has_worker_mutex = false;
    p->has_worker_cond = false;

    if (!p->file->get_error()) {
        p->nfft = p->fft->get_input_size();
        p->coss = (float *) malloc(p->nfft * sizeof(float));
        float cf = 2.0f * (float) M_PI / (p->nfft - 1.0f);
        for (int i = 0; i < p->nfft; ++i) {
            p->coss[i] = cosf(cf * i);
        }
        p->input_size = p->nfft * (NFFT * 2 + 1);
        p->input = (float *) malloc(p->input_size * sizeof(float));
        p->output = (float *) malloc(p->fft->get_output_size() * sizeof(float));
        p->file->start(channel, samples);
    }

    return p;
}

void spek_pipeline_start(struct spek_pipeline *p) {
    if (!!p->file->get_error()) {
        return;
    }

    p->input_pos = 0;
    p->worker_done = false;
    p->quit = false;

    p->has_reader_mutex = true;
    p->has_reader_cond = true;
    p->has_worker_mutex = true;
    p->has_worker_cond = true;

    p->has_reader_thread = (p->reader_thread = std::thread(reader_func, p), true);
    if (!p->has_reader_thread) {
        spek_pipeline_close(p);
    }
}

void spek_pipeline_close(struct spek_pipeline *p) {
    if (p->has_reader_thread) {
        p->quit = true;

        p->reader_thread.join();
        p->has_reader_thread = false;
    }
    if (p->has_worker_cond) {
        p->has_worker_cond = false;
    }
    if (p->has_worker_mutex) {
        p->has_worker_mutex = false;
    }
    if (p->has_reader_cond) {
        p->has_reader_cond = false;
    }
    if (p->has_reader_mutex) {
        p->has_reader_mutex = false;
    }

    if (p->output) {
        free(p->output);
        p->output = nullptr;
    }
    if (p->input) {
        free(p->input);
        p->input = nullptr;
    }
    if (p->coss) {
        free(p->coss);
        p->coss = nullptr;
    }

    p->file.reset();

    delete p;
}

std::string spek_pipeline_desc(const struct spek_pipeline *pipeline) {
    std::vector<std::string> items;

    if (!pipeline->file->get_codec_name().empty()) {
        items.push_back(pipeline->file->get_codec_name());
    }

    if (pipeline->file->get_bit_rate()) {
        items.push_back(std::string(
            QString("%1 kbps").arg((pipeline->file->get_bit_rate() + 500) / 1000).toUtf8().data()));
    }

    if (pipeline->file->get_sample_rate()) {
        items.push_back(
            std::string(QString("%1 Hz").arg(pipeline->file->get_sample_rate()).toUtf8().data()));
    }

    // Include bits per sample only if there is no bitrate.
    if (pipeline->file->get_bits_per_sample() && !pipeline->file->get_bit_rate()) {
        items.push_back(std::string(QString("%1 bit %2 bits")
                                        .arg(pipeline->file->get_bits_per_sample())
                                        .arg(pipeline->file->get_bits_per_sample())
                                        .toUtf8()
                                        .data()));
    }

    if (pipeline->file->get_channels()) {
        items.push_back(std::string(
            QString(
                // TRANSLATORS: first %d is the current channel, second %d is the total number.
                "channel %1 / %2")
                .arg(pipeline->channel + 1)
                .arg(pipeline->file->get_channels())
                .toUtf8()
                .data()));
    }

    if (pipeline->file->get_error() == AudioError::OK) {
        items.push_back(std::string(QString("W:%1").arg(pipeline->nfft).toUtf8().data()));

        std::string window_function_name;
        switch (pipeline->window_function) {
            case WINDOW_HANN:
                window_function_name = std::string("Hann");
                break;
            case WINDOW_HAMMING:
                window_function_name = std::string("Hamming");
                break;
            case WINDOW_BLACKMAN_HARRIS:
                window_function_name = std::string("Blackman–Harris");
                break;
            default:
                assert(false);
        }
        if (window_function_name.size()) {
            items.push_back("F:" + window_function_name);
        }
    }

    std::string desc;
    for (const auto &item : items) {
        if (!desc.empty()) {
            desc.append(", ");
        }
        desc.append(item);
    }

    QString error;
    switch (pipeline->file->get_error()) {
        case AudioError::CANNOT_OPEN_FILE:
            error = "Cannot open input file";
            break;
        case AudioError::NO_STREAMS:
            error = "Cannot find stream info";
            break;
        case AudioError::NO_AUDIO:
            error = "The file contains no audio streams";
            break;
        case AudioError::NO_DECODER:
            error = "Cannot find decoder";
            break;
        case AudioError::NO_DURATION:
            error = "Unknown duration";
            break;
        case AudioError::NO_CHANNELS:
            error = "No audio channels";
            break;
        case AudioError::CANNOT_OPEN_DECODER:
            error = "Cannot open decoder";
            break;
        case AudioError::BAD_SAMPLE_FORMAT:
            error = "Unsupported sample format";
            break;
        case AudioError::OK:
            break;
    }

    auto error_string = std::string(error.toUtf8().data());
    if (desc.empty()) {
        desc = error_string;
    } else if (pipeline->stream < pipeline->file->get_streams()) {
        desc = std::string(QString(
                               // TRANSLATORS: first %d is the stream number, second %d is the
                               // total number of streams, %s is the stream description.
                               "Stream %1 / %2: %3")
                               .arg(pipeline->stream + 1)
                               .arg(pipeline->file->get_streams())
                               .arg(desc.c_str())
                               .toUtf8()
                               .data());
    } else if (!error_string.empty()) {
        desc = std::string(
            // TRANSLATORS: first %s is the error message, second %s is stream description.
            QString("%1: %2").arg(error_string.c_str(), desc.c_str()).toUtf8().data());
    }

    return desc;
}

int spek_pipeline_streams(const struct spek_pipeline *pipeline) {
    return pipeline->file->get_streams();
}

int spek_pipeline_channels(const struct spek_pipeline *pipeline) {
    return pipeline->file->get_channels();
}

double spek_pipeline_duration(const struct spek_pipeline *pipeline) {
    return pipeline->file->get_duration();
}

int spek_pipeline_sample_rate(const struct spek_pipeline *pipeline) {
    return pipeline->file->get_sample_rate();
}

static void *reader_func(void *pp) {
    struct spek_pipeline *p = (spek_pipeline *) pp;

    p->has_worker_thread = (p->worker_thread = std::thread(worker_func, p), true);
    if (!p->has_worker_thread) {
        return nullptr;
    }

    int pos = 0, prev_pos = 0;
    int len;

    while ((len = p->file->read()) > 0) {
        if (p->quit)
            break;

        const float *buffer = p->file->get_buffer();
        while (len-- > 0) {
            p->input[pos] = *buffer++;
            pos = (pos + 1) % p->input_size;

            // Wake up the worker if we have enough data.
            if ((pos > prev_pos ? pos : pos + p->input_size) - prev_pos == p->nfft * NFFT) {
                reader_sync(p, prev_pos = pos);
            }
        }
        assert(len == -1);
    }

    if (pos != prev_pos) {
        // Process the remaining data.
        reader_sync(p, pos);
    }

    // Force the worker to quit.
    reader_sync(p, -1);
    p->worker_thread.join();
    p->has_worker_thread = false;

    // Notify the client.
    p->cb(p->fft->get_output_size(), -1, nullptr, p->cb_data);
    return nullptr;
}

static void reader_sync(struct spek_pipeline *p, int pos) {
    {
        std::unique_lock<std::mutex> lock(p->reader_mutex);
        p->reader_cond.wait(lock, [&]() { return p->worker_done; });
        p->worker_done = false;
    }

    p->worker_mutex.lock();
    p->input_pos = pos;
    p->worker_cond.notify_all();
    p->worker_mutex.unlock();
}

static float get_window(enum window_function f, int i, float *coss, int n) {
    switch (f) {
        case WINDOW_HANN:
            return 0.5f * (1.0f - coss[i]);
        case WINDOW_HAMMING:
            return 0.53836f - 0.46164f * coss[i];
        case WINDOW_BLACKMAN_HARRIS:
            return 0.35875f - 0.48829f * coss[i] + 0.14128f * coss[2 * i % n] -
                   0.01168f * coss[3 * i % n];
        default:
            assert(false);
            return 0.0f;
    }
}

static void *worker_func(void *pp) {
    struct spek_pipeline *p = (spek_pipeline *) pp;

    int sample = 0;
    int64_t frames = 0;
    int64_t num_fft = 0;
    int64_t acc_error = 0;
    int head = 0, tail = 0;
    int prev_head = 0;

    memset(p->output, 0, sizeof(float) * p->fft->get_output_size());

    while (true) {
        p->reader_mutex.lock();
        p->worker_done = true;
        p->reader_cond.notify_all();
        p->reader_mutex.unlock();

        {
            std::unique_lock<std::mutex> lock(p->worker_mutex);
            p->worker_cond.wait(lock, [&]() { return tail != p->input_pos; });
            tail = p->input_pos;
        }

        if (tail == -1) {
            return nullptr;
        }

        while (true) {
            head = (head + 1) % p->input_size;
            if (head == tail) {
                head = prev_head;
                break;
            }
            frames++;

            // If we have enough frames for an FFT or we have
            // all frames required for the interval run and FFT.
            bool int_full = acc_error < p->file->get_error_base() &&
                            frames == p->file->get_frames_per_interval();
            bool int_over = acc_error >= p->file->get_error_base() &&
                            frames == 1 + p->file->get_frames_per_interval();

            if (frames % p->nfft == 0 || ((int_full || int_over) && num_fft == 0)) {
                prev_head = head;
                for (int i = 0; i < p->nfft; i++) {
                    float val = p->input[(p->input_size + head - p->nfft + i) % p->input_size];
                    val *= get_window(p->window_function, i, p->coss, p->nfft);
                    p->fft->set_input(i, val);
                }
                p->fft->execute();
                num_fft++;
                for (int i = 0; i < p->fft->get_output_size(); i++) {
                    p->output[i] += p->fft->get_output(i);
                }
            }

            // Do we have the FFTs for one interval?
            if (int_full || int_over) {
                if (int_over) {
                    acc_error -= p->file->get_error_base();
                } else {
                    acc_error += p->file->get_error_per_interval();
                }

                for (int i = 0; i < p->fft->get_output_size(); i++) {
                    p->output[i] /= num_fft;
                }

                if (sample == p->samples)
                    break;
                p->cb(p->fft->get_output_size(), sample++, p->output, p->cb_data);

                memset(p->output, 0, sizeof(float) * p->fft->get_output_size());
                frames = 0;
                num_fft = 0;
            }
        }
    }
}
