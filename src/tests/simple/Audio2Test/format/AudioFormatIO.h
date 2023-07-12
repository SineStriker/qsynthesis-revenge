//
// Created by Crs_1 on 2023/7/12.
//

#ifndef CHORUSKIT_AUDIOFORMATIO_H
#define CHORUSKIT_AUDIOFORMATIO_H

#include <QIODevice>

#include "utils/IErrorStringProvider.h"

class SndfileHandle;
class QIODevice;

class AudioFormatIOPrivate;

class AudioFormatIO: public IErrorStringProvider {
    Q_DECLARE_PRIVATE(AudioFormatIO)
public:
    explicit AudioFormatIO(QIODevice *stream = nullptr);
    ~AudioFormatIO();

    void setStream(QIODevice *stream);
    QIODevice *stream() const;

    typedef QIODevice::OpenMode OpenMode;
    typedef QIODevice::OpenModeFlag OpenModeFlag;

    enum MajorFormat {
        WAV = 0x010000,
        AIFF = 0x020000,
        AU = 0x030000,
        RAW = 0x040000,
        PAF = 0x050000,
        SVX = 0x060000,
        NIST = 0x070000,
        VOC = 0x080000,
        IRCAM = 0x0A0000,
        W64 = 0x0B0000,
        MAT4 = 0x0C0000,
        MAT5 = 0x0D0000,
        PVF = 0x0E0000,
        XI = 0x0F0000,
        HTK = 0x100000,
        SDS = 0x110000,
        AVR = 0x120000,
        WAVEX = 0x130000,
        SD2 = 0x160000,
        FLAC = 0x170000,
        CAF = 0x180000,
        WVE = 0x190000,
        OGG = 0x200000,
        MPC2K = 0x210000,
        RF64 = 0x220000,
        MPEG = 0x230000,

        MajorFormatMask = 0x0FFF0000
    };

    enum Subtype {
        PCM_S8 = 0x0001,
        PCM_16 = 0x0002,
        PCM_24 = 0x0003,
        PCM_32 = 0x0004,
        PCM_U8 = 0x0005,
        FLOAT = 0x0006,
        DOUBLE = 0x0007,
        ULAW = 0x0010,
        ALAW = 0x0011,
        IMA_ADPCM = 0x0012,
        MS_ADPCM = 0x0013,
        GSM610 = 0x0020,
        VOX_ADPCM = 0x0021,
        NMS_ADPCM_16 = 0x0022,
        NMS_ADPCM_24 = 0x0023,
        NMS_ADPCM_32 = 0x0024,
        G721_32 = 0x0030,
        G723_24 = 0x0031,
        G723_40 = 0x0032,
        DWVW_12 = 0x0040,
        DWVW_16 = 0x0041,
        DWVW_24 = 0x0042,
        DWVW_N = 0x0043,
        DPCM_8 = 0x0050,
        DPCM_16 = 0x0051,
        VORBIS = 0x0060,
        OPUS = 0x0064,
        ALAC_16 = 0x0070,
        ALAC_20 = 0x0071,
        ALAC_24 = 0x0072,
        ALAC_32 = 0x0073,
        MPEG_LAYER_I = 0x0080,
        MPEG_LAYER_II = 0x0081,
        MPEG_LAYER_III = 0x0082,

        SubtypeMask = 0x0000FFFF
    };

    enum ByteOrder {
        DefaultOrder = 0x00000000,
        LittleEndian = 0x10000000,
        BigEndian = 0x20000000,
        SystemOrder = 0x30000000,

        ByteOrderMask = 0x30000000
    };

    bool open(OpenMode openMode);
    bool open(OpenMode openMode, int format, int channels, double sampleRate);
    OpenMode openMode() const;
    void close();

    int channels() const;
    double sampleRate() const;
    int format() const;
    MajorFormat majorFormat() const;
    Subtype subType() const;
    ByteOrder byteOrder() const;
    qint64 length() const;

    enum MetaData {
        Title = 0x01,
        Copyright = 0x02,
        Software = 0x03,
        Artist = 0x04,
        Comment = 0x05,
        Date = 0x06,
        Album = 0x07,
        License = 0x08,
        TrackNumber = 0x09,
        Genre = 0x10
    };

    void setMetaData(MetaData metaDataType, const QString &str);
    QString getMetaData(MetaData metaDataType) const;

    qint64 read(float *ptr, qint64 length);
    qint64 write(float *ptr, qint64 length);
    qint64 seek(qint64 pos);

    struct SubtypeInfo {
        Subtype subtype;
        QString name;
    };

    struct FormatInfo {
        MajorFormat majorFormat;
        QString name;
        QString extension;
        QList<SubtypeInfo> subtypes;
        QList<ByteOrder> byteOrders;
    };
    static QList<FormatInfo> availableFormats();

protected:
    explicit AudioFormatIO(AudioFormatIOPrivate &d);
    QScopedPointer<AudioFormatIOPrivate> d_ptr;
};



#endif // CHORUSKIT_AUDIOFORMATIO_H
