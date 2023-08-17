//
// Created by Crs_1 on 2023/8/18.
//

#include "AudioSampleConverter.h"

#include <QtEndian>
#include <QSysInfo>

static const double factor16 = (double)0x7fffL + 0.49999;
static const double factor24 = (double)0x7fffffL + 0.49999;
static const double factor32 = (double)0x7fffffffL + 0.49999;

void AudioSampleConverter::convertFloatToInt16(void *dest, const float *src, qint64 length, bool isLittleEndian) {
    auto p = reinterpret_cast<qint16 *>(dest);
    while(--length >= 0)
        *p++ = isLittleEndian ? (qint16)qToLittleEndian((double)(*src++) * factor16) : (qint16)qToBigEndian((double)(*src++) * factor16);
}
void AudioSampleConverter::convertFloatToInt24(void *dest, const float *src, qint64 length, bool isLittleEndian) {
    long a;
    char* b = (char*)dest;
    char* aa = (char*)&a;
    while(--length >= 0){
        if(isLittleEndian) {
            a = (qint32)qToLittleEndian((double)(*src++) * factor24);
            *b++ = aa[0];
            *b++ = aa[1];
            *b++ = aa[2];
        } else {
            a = (qint32)qToBigEndian((double)(*src++) * factor24);
            *b++ = aa[3];
            *b++ = aa[2];
            *b++ = aa[1];
        }
    }
}
void AudioSampleConverter::convertFloatToInt32(void *dest, const float *src, qint64 length, bool isLittleEndian) {
    auto p = reinterpret_cast<qint32 *>(dest);
    while(--length >= 0)
        *p++ = isLittleEndian ? (qint32)qToLittleEndian((double)(*src++) * factor32) : (qint32)qToBigEndian((double)(*src++) * factor32);
}
void AudioSampleConverter::convertFloatToFloat32(void *dest, const float *src, qint64 length, bool isLittleEndian) {
    if(isLittleEndian) {
        qToLittleEndian<float>(src, length, dest);
    } else {
        qToBigEndian<float>(src, length, dest);
    }
}
void AudioSampleConverter::convertFloatToFloat64(void *dest, const float *src, qint64 length, bool isLittleEndian) {
    auto p = reinterpret_cast<double *>(dest);
    while(--length >= 0)
        *p++ = isLittleEndian ? qToLittleEndian(*src++) : qToBigEndian(*src++);
}
