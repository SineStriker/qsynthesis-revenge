#include "VstClientPlugin.h"

#include <QApplication>
#include <QThread>

#include "CoreApi/ILoader.h"
#include "IpcClient.h"
#include "IpcReceiveChannel.h"
#include "IpcSendChannel.h"

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include <QMessageBox>
#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>


namespace Vst {

    using namespace Core;

    static void sineWave(double sampleRate, int position, int size, float *buf) {
        double freq = 440 / sampleRate;
        for(int x = 0; x < size; x++) {
            buf[x] = sin(2 * 3.14159265358979323846 * freq * (position + x));
        }
    }

    namespace Internal {

        VstClientPlugin::VstClientPlugin() {
        }

        VstClientPlugin::~VstClientPlugin() {
        }


        bool VstClientPlugin::initialize(const QStringList &arguments, QString *errorMessage) {
            // Add resources
            qIDec->addTranslationPath(pluginSpec()->location() + "/translations");

            auto splash = qobject_cast<QSplashScreen *>(ILoader::instance()->getFirstObject("choruskit_init_splash"));
            if (splash) {
                splash->showMessage(tr("Initializing vst support..."));
            }

            auto *mainIpcClient = new IpcClient("77F6E993-671E-4283-99BE-C1CD1FF5C09E", this);
            auto *mainIpcReceiveChannel = new IpcReceiveChannel(mainIpcClient);
            connect(mainIpcReceiveChannel, &IpcReceiveChannel::received, this, [=](quint8 signal, const QByteArray &payload, QByteArray &ret){
                QMessageBox::information(nullptr, "Test", QString::number(signal));
                QDataStream out(&ret, QIODevice::WriteOnly);
                out << true;
            });
            mainIpcClient->open();
            auto *processingIpcClient = new IpcClient("77F6E993-671E-4283-99BE-C1CD1FF5C09Eprocessing", this);
            auto *processingIpcReceiveChannel = new IpcReceiveChannel(processingIpcClient);
            connect(processingIpcReceiveChannel, &IpcReceiveChannel::received, this, [=](quint8 signal, const QByteArray &payload, QByteArray &ret){
                if(signal == 3) {
                    QDataStream in(payload);
                    double sampleRate;
                    qint64 position;
                    qint32 bufferSize;
                    bool isPlaying;
                    qint32 numOutputs;
                    in >> sampleRate >> position >> bufferSize >> isPlaying >> numOutputs;
                    auto *buf = (float*) processingIpcClient->sharedData();
                    sineWave(sampleRate, position, bufferSize, buf);
                    sineWave(sampleRate, position, bufferSize, buf + bufferSize);
                    QDataStream out(&ret, QIODevice::WriteOnly);
                    out << true;
                }
            });
            processingIpcClient->open();

            auto actionMgr = ICore::instance()->actionSystem();

            // Add basic windows and add-ons

            return true;
        }

        void VstClientPlugin::extensionsInitialized() {
        }

        bool VstClientPlugin::delayedInitialize() {
            return false;
        }

        QObject *VstClientPlugin::remoteCommand(const QStringList &options, const QString &workingDirectory,
                                                const QStringList &args) {
            return nullptr;
        }

    }

}