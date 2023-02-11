#ifndef IAUDIOENCODER_H
#define IAUDIOENCODER_H

#include <QObject>

#include "INamePlugin.h"
#include "QsMediaGlobal.h"

class QSMEDIA_API IAudioEncoder : public INamePlugin {
    Q_OBJECT
public:
    explicit IAudioEncoder(QObject *parent = nullptr);
    ~IAudioEncoder();
};

#define IAudioEncoder_IID "QSynthesis.Plugin.Media.AudioEncoder"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(IAudioEncoder, IAudioEncoder_IID)
QT_END_NAMESPACE

#endif // IAUDIOENCODER_H
