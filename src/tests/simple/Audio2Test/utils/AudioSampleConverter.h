//
// Created by Crs_1 on 2023/8/18.
//

#ifndef CHORUSKIT_AUDIOSAMPLECONVERTER_H
#define CHORUSKIT_AUDIOSAMPLECONVERTER_H

#include <QtGlobal>

class AudioSampleConverter {
public:
    static void convertFloatToInt16(void *dest, const float *src, qint64 length, bool isLittleEndian);
    static void convertFloatToInt24(void *dest, const float *src, qint64 length, bool isLittleEndian);
    static void convertFloatToInt32(void *dest, const float *src, qint64 length, bool isLittleEndian);
    static void convertFloatToFloat32(void *dest, const float *src, qint64 length, bool isLittleEndian);
    static void convertFloatToFloat64(void *dest, const float *src, qint64 length, bool isLittleEndian);
};



#endif // CHORUSKIT_AUDIOSAMPLECONVERTER_H
