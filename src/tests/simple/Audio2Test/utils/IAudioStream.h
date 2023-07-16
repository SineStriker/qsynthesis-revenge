//
// Created by Crs_1 on 2023/7/10.
//

#ifndef CHORUSKIT_IAUDIOSTREAM_H
#define CHORUSKIT_IAUDIOSTREAM_H

#include <qglobal.h>

class IAudioStream {
public:
    virtual bool open(qint64 bufferSize, double sampleRate);
    bool isOpen() const;
    virtual void close();

    qint64 bufferSize() const;
    double sampleRate() const;

private:
    qint64 m_bufferSize = 0;
    double m_sampleRate = 0;
    bool m_isOpened = false;
};



#endif // CHORUSKIT_IAUDIOSTREAM_H
