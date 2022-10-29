#ifndef IAUDIODECODER_H
#define IAUDIODECODER_H

#include <QIODevice>

#include "NAudio/WaveDecoder.h"

#include "qsmedia_global.h"

class QSMEDIA_API IAudioDecoder : public QObject, public WaveDecoder {
    Q_OBJECT
public:
    IAudioDecoder(const WaveArguments &args, QObject *parent = nullptr);
    ~IAudioDecoder();

public:
};

#define IAudioDecoder_IID "QSynthesis.Plugin.Media.AudioDecoder"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IAudioDecoder, IAudioDecoder_IID)
QT_END_NAMESPACE

#endif // IAUDIODECODER_H
