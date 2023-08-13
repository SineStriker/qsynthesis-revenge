//
// Created by Crs_1 on 2023/7/25.
//

#ifndef CHORUSKIT_VSTHELPER_H
#define CHORUSKIT_VSTHELPER_H

#include <QObject>
#include <extensionsystem/iplugin.h>
class QRemoteObjectHost;

namespace Vst::Internal {

    class VstBridge;
    class VstClientAddOn;

    struct ConnectionStatus {
        bool isRemoting = false;
        bool isConnected = false;
        QString hostName;
        bool isProcessing = false;
        double sampleRate = 0;
        int channelCount = 0;
        int bufferSize = 0;
    };

    class VstHelper: public QObject {
        Q_OBJECT
    public:
        VstBridge *vstBridge;
        QRemoteObjectHost *srcNode;
        ConnectionStatus connectionStatus;
        bool isLoadFromInitialization = false;
        VstClientAddOn *addOn = nullptr;

        explicit VstHelper(QObject *parent = nullptr);
        ~VstHelper();
        static VstHelper *instance();

        bool startRemoting();
        void stopRemoting();

        void notifyUpdateConnectionStatus();
        void notifyUpdateStatistics();

        static void generateVstConfig();
        static QString globalUuid();

    signals:
        void connectionStatusChanged();
        void statisticsChanged();
    };

} // Internal

#endif // CHORUSKIT_VSTHELPER_H
