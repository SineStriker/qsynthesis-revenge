#ifndef QSMEDIA_GLOBAL_H
#define QSMEDIA_GLOBAL_H

#include <QtGlobal>

#ifndef QSMEDIA_API
#ifdef QSMEDIA_STATIC
#define QSMEDIA_API
#else
#ifdef QSMEDIA_LIBRARY
#define QSMEDIA_API Q_DECL_EXPORT
#else
#define QSMEDIA_API Q_DECL_IMPORT
#endif
#endif
#endif

namespace FF {

    enum SampleFormat {
        FMT_U8,  ///< unsigned 8 bits
        FMT_S16, ///< signed 16 bits
        FMT_S32, ///< signed 32 bits
        FMT_FLT, ///< float
        FMT_DBL, ///< double

        FMT_U8P,  ///< unsigned 8 bits, planar
        FMT_S16P, ///< signed 16 bits, planar
        FMT_S32P, ///< signed 32 bits, planar
        FMT_FLTP, ///< float, planar
        FMT_DBLP, ///< double, planar
        FMT_S64,  ///< signed 64 bits
        FMT_S64P, ///< signed 64 bits, planar
    };

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

}; // namespace FF

#endif // QSMEDIA_GLOBAL_H
