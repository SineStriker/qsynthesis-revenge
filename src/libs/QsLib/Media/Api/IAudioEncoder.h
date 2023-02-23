#ifndef IAUDIOENCODER_H
#define IAUDIOENCODER_H

#include <QObject>

#include "QMNamePlugin.h"
#include "QsMediaGlobal.h"

QSAPI_BEGIN_NAMESPACE

class QSMEDIA_API IAudioEncoder : public QMNamePlugin {
    Q_OBJECT
public:
    explicit IAudioEncoder(QObject *parent = nullptr);
    ~IAudioEncoder();
};

QSAPI_END_NAMESPACE

#define QsApi_IAudioEncoder_IID "QSynthesis.Plugin.Media.AudioEncoder"

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(QsApi::IAudioEncoder, QsApi_IAudioEncoder_IID)
QT_END_NAMESPACE

#endif // IAUDIOENCODER_H
