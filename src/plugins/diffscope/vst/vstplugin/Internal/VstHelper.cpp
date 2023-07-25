//
// Created by Crs_1 on 2023/7/25.
//

#include "VstHelper.h"
#include "AddOn/VstClientAddOn.h"
#include "CoreApi/ILoader.h"
#include "VstClientPlugin.h"
#include <QApplication>
#include <QMDecoratorV2.h>
#include <QMSystem.h>
#include <QMessageBox>
#include <QThread>
#include <QtRemoteObjects>
#include <cmath>
#include <coreplugin/ICore.h>
#include <extensionsystem/pluginspec.h>

#include <ICore.h>

#include "Connection//VstBridge.h"

namespace Vst::Internal {

    static VstHelper *m_instance = nullptr;

    VstHelper::VstHelper(QObject *parent) : QObject(parent) {
        m_instance = this;
        vstBridge = new VstBridge(this);
        srcNode = new QRemoteObjectHost(QUrl("local:" + globalUuid()), this);
    }
    VstHelper::~VstHelper() {
        m_instance = nullptr;
    }
    VstHelper *VstHelper::instance() {
        return m_instance;
    }
    bool VstHelper::startRemoting() {
        return connectionStatus.isRemoting = srcNode->enableRemoting(vstBridge);
    }
    void VstHelper::stopRemoting() {
        vstBridge->finalizeVst();
        srcNode->disableRemoting(vstBridge);
        connectionStatus.isRemoting = false;
    }
    void VstHelper::generateVstConfig() {
        QFile configFile(QMFs::appDataPath() + "/ChorusKit/DiffScope/vstconfig.txt");
        configFile.open(QFile::WriteOnly | QFile::Text);
        if (configFile.isOpen()) {
            QTextStream stream(&configFile);
            stream << QDir::toNativeSeparators(QApplication::applicationDirPath()) << QDir::separator() << Qt::endl;
            stream << "vstbridge."
                   <<
#ifdef Q_OS_WINDOWS
                "dll"
#elif defined(Q_OS_MAC)
                "dylib"
#else
                "so"
#endif
                   << Qt::endl;
            stream << QDir::toNativeSeparators(QApplication::applicationFilePath()) << Qt::endl;
            stream.flush();
            configFile.close();
        } else {
            qDebug() << "VST Plugin: Cannot write vstconfig.txt " << configFile.errorString();
        }
    }
    QString VstHelper::globalUuid() {
        return "77F6E993-671E-4283-99BE-C1CD1FF5C09E";
    }
    QString VstHelper::statusText() const {
        QString yesText = tr("Yes");
        QString noText = tr("No");
        QString naText = "N/A";
        return
            tr("Enabled: ") + (connectionStatus.isRemoting ? yesText : noText) + "\n" +
            tr("Connected: ") + (connectionStatus.isConnected ? yesText : noText) + "\n" +
            tr("Processing: ") + (connectionStatus.isProcessing ? yesText: noText) + "\n" +
            tr("Sample rate: ") + (connectionStatus.isProcessing ? QString::number(connectionStatus.sampleRate) : naText) + "\n" +
            tr("Channel count: ") + (connectionStatus.isProcessing ? QString::number(connectionStatus.channelCount) : naText) + "\n" +
            tr("Buffer size: ") + (connectionStatus.isProcessing ? QString::number(connectionStatus.bufferSize) : naText);
    }
} // Internal