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
#include "Settings/VstSettingPage.h"

namespace Vst::Internal {

    static VstHelper *m_instance = nullptr;
    static QString mainKey;

    VstHelper::VstHelper(QObject *parent) : QObject(parent) {
        m_instance = this;
        mainKey = VstSettingPage::mainKey();
        vstBridge = new VstBridge(this);
        srcNode = new QRemoteObjectHost(QUrl("local:" + globalUuid()), this);
    }
    VstHelper::~VstHelper() {
        delete srcNode;
        delete vstBridge;
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
            stream << VstSettingPage::ipcTimeout() << Qt::endl;
            stream << VstSettingPage::mainKey() << Qt::endl;
            stream.flush();
            configFile.close();
        } else {
            qDebug() << "VST Plugin: Cannot write vstconfig.txt " << configFile.errorString();
        }
    }
    QString VstHelper::globalUuid() {
        return mainKey;
    }
    void VstHelper::notifyUpdateConnectionStatus() {
        emit connectionStatusChanged();
    }
    void VstHelper::notifyUpdateStatistics() {
        emit statisticsChanged();
    }
} // Internal