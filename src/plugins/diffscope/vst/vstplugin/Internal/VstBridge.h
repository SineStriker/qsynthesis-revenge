//
// Created by Crs_1 on 2023/5/26.
//

#ifndef CHORUSKIT_VSTBRIDGE_H
#define CHORUSKIT_VSTBRIDGE_H

#include "rep_VstBridge_source.h"

class QLocalSocket;

namespace Vst::Internal {

    const QString GLOBAL_UUID = "77F6E993-671E-4283-99BE-C1CD1FF5C09E";

    class VstClientPlugin;
    class VstClientAddOn;

    class VstBridge: public VstBridgeSource {
        Q_OBJECT
        friend class VstClientPlugin;
        friend class VstClientAddOn;
        explicit VstBridge(QObject *parent = nullptr);
    public:

        ~VstBridge();
        static VstBridge *instance();

        bool initializeVst() override;
        void finalizeVst() override;
        QByteArray saveDataFromEditor() override;
        bool openDataToEditor(const QByteArray &data) override;
        void showWindow() override;
        void hideWindow() override;
        void notifySwitchAudioBuffer(qint64 position, int bufferSize, int channelCount, bool isPlaying, bool isRealtime) override;

    private:
        QLocalSocket *m_alivePipe;
        VstClientAddOn *m_clientAddOn = nullptr;
    };

} // Vst

#endif // CHORUSKIT_VSTBRIDGE_H
