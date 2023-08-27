//
// Created by Crs_1 on 2023/7/12.
//

#include "AudioFormatIO.h"
#include "AudioFormatIO_p.h"

#include <sndfile.hh>

#include <QDebug>
#include <QIODevice>

#define TEST_IS_OPEN(ret) \
if(d->sf.isNull()) { \
    qDebug() << "AudioFormatIO: Not open."; \
    return ret; \
} \

AudioFormatIO::AudioFormatIO(QIODevice *stream): AudioFormatIO(*new AudioFormatIOPrivate) {
      setStream(stream);
}
AudioFormatIO::AudioFormatIO(AudioFormatIOPrivate &d): d_ptr(&d) {
    d.q_ptr = this;
}
AudioFormatIO::~AudioFormatIO() {
    close();
}
void AudioFormatIO::setStream(QIODevice *stream) {
    Q_D(AudioFormatIO);
    if(d->openMode) {
        qWarning() << "AudioFormatIO: Cannot set stream when AudioFormatIO is open.";
        return;
    }
    d->stream = stream;
}
QIODevice *AudioFormatIO::stream() const {
    Q_D(const AudioFormatIO);
    return d->stream;
}

int64_t AudioFormatIOPrivate::sfVioGetFilelen() const {
    return stream->size();
}
int64_t AudioFormatIOPrivate::sfVioSeek(int64_t offset, int whence) const {
    if(whence == SF_SEEK_CUR) offset += stream->pos();
    else if(whence == SF_SEEK_END) offset += stream->size();
    if(offset != stream->pos()) {
        if(!stream->seek(offset))
            return -1;
    }
    return stream->pos();
}
int64_t AudioFormatIOPrivate::sfVioRead(void *ptr, int64_t count) const {
    return stream->read((char *)ptr, count);
}
int64_t AudioFormatIOPrivate::sfVioWrite(const void *ptr, int64_t count) const {
    return stream->write((char *)ptr, count);
}
int64_t AudioFormatIOPrivate::sfVioTell() const {
    return stream->pos();
}
static SF_VIRTUAL_IO sfVio = {
    [](void *d) {
        return reinterpret_cast<AudioFormatIOPrivate *>(d)->sfVioGetFilelen();
    },
    [](int64_t offset, int whence, void *d) {
        return reinterpret_cast<AudioFormatIOPrivate *>(d)->sfVioSeek(offset, whence);
    },
    [](void *ptr, int64_t count, void *d) {
        return reinterpret_cast<AudioFormatIOPrivate *>(d)->sfVioRead(ptr, count);
    },
    [](const void *ptr, int64_t count, void *d) {
        return reinterpret_cast<AudioFormatIOPrivate *>(d)->sfVioWrite(ptr, count);
    },
    [](void *d) {
        return reinterpret_cast<AudioFormatIOPrivate *>(d)->sfVioTell();
    }
};

bool AudioFormatIO::open(QIODevice::OpenMode openMode) {
    return open(openMode, 0, 0, 0);
}
bool AudioFormatIO::open(QIODevice::OpenMode openMode, int format, int channels, double sampleRate) {
    Q_D(AudioFormatIO);
    close();
    int sfOpenMode = 0;
    if(openMode.testFlag(QIODevice::ReadOnly)) {
        sfOpenMode |= SFM_READ;
    }
    if(openMode.testFlag(QIODevice::WriteOnly)) {
        sfOpenMode |= SFM_WRITE;
    }
    if(sfOpenMode == 0) {
        qWarning() << "AudioFormatIO: Cannot open because access mode is not specified.";
        return false;
    }
    if(!d->stream) {
        qWarning() << "AudioFormatIO: Cannot open because stream is null.";
        return false;
    }
    if(!d->stream->open(openMode)) {
        setErrorString(d->stream->errorString());
        return false;
    }
    d->sf.reset(new SndfileHandle(sfVio, d, sfOpenMode, format, channels, (int)sampleRate));
    if(!d->sf->rawHandle()) {
        setErrorString(d->sf->strError());
        return false;
    }
    d->openMode = openMode;
    return true;
}
QIODevice::OpenMode AudioFormatIO::openMode() const {
    Q_D(const AudioFormatIO);
    return d->openMode;
}
void AudioFormatIO::close() {
    Q_D(AudioFormatIO);
    d->sf.reset();
    if(d->stream) d->stream->close();
    d->openMode = QIODevice::NotOpen;
    clearErrorString();
}
int AudioFormatIO::channels() const {
    Q_D(const AudioFormatIO);
    TEST_IS_OPEN(0)
    return d->sf->channels();
}
double AudioFormatIO::sampleRate() const {
    Q_D(const AudioFormatIO);
    TEST_IS_OPEN(0)
    return d->sf->samplerate();
}
int AudioFormatIO::format() const {
    Q_D(const AudioFormatIO);
    TEST_IS_OPEN(0)
    return d->sf->format();
}
AudioFormatIO::MajorFormat AudioFormatIO::majorFormat() const {
    return (MajorFormat)(format() & MajorFormatMask);
}
AudioFormatIO::Subtype AudioFormatIO::subType() const {
    return (Subtype)(format() & SubtypeMask);
}
AudioFormatIO::ByteOrder AudioFormatIO::byteOrder() const {
    return (ByteOrder)(format() & ByteOrderMask);
}
qint64 AudioFormatIO::length() const {
    Q_D(const AudioFormatIO);
    TEST_IS_OPEN(0)
    return d->sf->frames();
}
void AudioFormatIO::setMetaData(AudioFormatIO::MetaData metaDataType, const QString &str) {
    Q_D(AudioFormatIO);
    TEST_IS_OPEN(void())
    d->sf->setString(metaDataType, str.toUtf8().data());
}
QString AudioFormatIO::getMetaData(AudioFormatIO::MetaData metaDataType) const {
    Q_D(const AudioFormatIO);
    TEST_IS_OPEN({})
    return QString::fromUtf8(d->sf->getString(metaDataType));
}
qint64 AudioFormatIO::read(float *ptr, qint64 length) {
    Q_D(AudioFormatIO);
    TEST_IS_OPEN(0)
    return d->sf->readf(ptr, length);
}
qint64 AudioFormatIO::write(const float *ptr, qint64 length) {
    Q_D(AudioFormatIO);
    TEST_IS_OPEN(0)
    return d->sf->writef(ptr, length);
}
qint64 AudioFormatIO::seek(qint64 pos) {
    Q_D(AudioFormatIO);
    TEST_IS_OPEN(0)
    return d->sf->seek(pos, SF_SEEK_SET);
}
qint64 AudioFormatIO::pos() {
    Q_D(AudioFormatIO);
    TEST_IS_OPEN(0)
    return d->sf->seek(0, SF_SEEK_CUR);
}
QList<AudioFormatIO::FormatInfo> AudioFormatIO::availableFormats() {
    QList<FormatInfo> formatInfoList;
    SF_INFO sfinfo = {} ;
    int majorTypeCnt, subTypeCnt;
    sf_command (nullptr, SFC_GET_FORMAT_MAJOR_COUNT, &majorTypeCnt, sizeof (int)) ;
    sf_command (nullptr, SFC_GET_FORMAT_SUBTYPE_COUNT, &subTypeCnt, sizeof (int)) ;
    sfinfo.channels = 1 ;
    for (int i = 0 ; i < majorTypeCnt ; i++) {
        SF_FORMAT_INFO info = {};
        FormatInfo formatInfo = {};
        info.format = i ;
        sf_command (nullptr, SFC_GET_FORMAT_MAJOR, &info, sizeof (info)) ;
        formatInfo.majorFormat = (MajorFormat)info.format;
        formatInfo.name = info.name;
        formatInfo.extension = info.extension;
        for (int j = 0 ; j < subTypeCnt ; j++) {
            info.format = j ;
            sf_command (nullptr, SFC_GET_FORMAT_SUBTYPE, &info, sizeof (info)) ;
            sfinfo.format = formatInfo.majorFormat | info.format ;
            if (sf_format_check (&sfinfo)) {
                formatInfo.subtypes.append({(Subtype)info.format, info.name});
            }
        }
        formatInfo.byteOrders.append(DefaultOrder);
        sfinfo.format = formatInfo.majorFormat | formatInfo.subtypes[0].subtype | LittleEndian;
        if(sf_format_check(&sfinfo)) formatInfo.byteOrders.append(LittleEndian);
        sfinfo.format = formatInfo.majorFormat | formatInfo.subtypes[0].subtype | BigEndian;
        if(sf_format_check(&sfinfo)) formatInfo.byteOrders.append(BigEndian);
        if(formatInfo.byteOrders.size() == 3) formatInfo.byteOrders.append(SystemOrder);
        formatInfoList.append(formatInfo);
    }
    return formatInfoList;
}
