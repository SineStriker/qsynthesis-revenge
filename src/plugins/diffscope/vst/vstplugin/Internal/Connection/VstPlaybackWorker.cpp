//
// Created by Crs_1 on 2023/8/3.
//
#include "VstPlaybackWorker.h"

#include <cmath>

#include <QSharedMemory>
#include <QThread>
#include <QSystemSemaphore>

#include "../../VstProcessData.h"
#include "VstHelper.h"

namespace Vst::Internal {

    VstPlaybackWorker::VstPlaybackWorker(QSharedMemory *processDataSharedMemory, QObject *parent):
          m_processDataSharedMemory(processDataSharedMemory),
          m_processBufferSharedMemory(new QSharedMemory(VstHelper::globalUuid() + "buffer", this)),
          m_processCallMutex(new QSystemSemaphore(VstHelper::globalUuid() + "process_call")),
          QObject(parent)
    {

    }

    static qint64 pos = 0;

    static void genSineWave(qint64 position, qint64 size, float freq, float sampleRate, float *output) {
        double omega = 2 * 3.14159265358979323846 * freq / sampleRate;
        for(int i = 0; i < size; i++) {
            output[i] = sin(omega * (position + i));
        }
    }

    void VstPlaybackWorker::start() {
        m_requestFinish = false;
        m_processData = reinterpret_cast<VstProcessData *>(m_processDataSharedMemory->data());
        while(!m_requestFinish) {
            m_processCallMutex->acquire();
            if(m_processData->flag == VstProcessData::ProcessInitializationRequest) {
                if(m_processData->sampleRate != 0) {
                    if(initialize(m_processData->sampleRate, m_processData->bufferSize, m_processData->channelCount)) {
                        m_processData->flag = VstProcessData::ProcessInitializationResponse;
                    } else {
                        m_processData->flag = VstProcessData::ProcessInitializationError;
                    }
                } else {
                    finalize();
                    m_processData->flag = VstProcessData::ProcessInitializationResponse;
                }
            } else if(m_processData->flag == VstProcessData::BufferSwitchRequest) {
                if(work(m_bufferSwitchData->isRealtime, m_bufferSwitchData->isPlaying, m_bufferSwitchData->position, m_bufferSwitchData->size, m_bufferSwitchData->channelCount, m_planarOutputData.constData())) {
                    m_processData->flag = VstProcessData::BufferSwitchFinished;
                } else {
                    m_processData->flag = VstProcessData::BufferSwitchError;
                }
            }
            m_processCallMutex->release();
        }
        m_processData = nullptr;
        finalize();
    }
    void VstPlaybackWorker::quit() {
        m_requestFinish = true;
    }
    bool VstPlaybackWorker::initialize(float sampleRate, int bufferSize, int channelCount) {
        qDebug() << "VstPlaybackWorker: initialize (" << channelCount << bufferSize << sampleRate << ")";

        // Setup shared memory
        if(m_processBufferSharedMemory->isAttached()) {
            m_processBufferSharedMemory->detach();
        }
        if(!m_processBufferSharedMemory->attach()) {
            return false;
        }
        m_bufferSwitchData = reinterpret_cast<VstBufferSwitchData *>(m_processBufferSharedMemory->data());
        m_planarOutputData.resize(channelCount);
        for(int i = 0; i < channelCount; i++) {
            m_planarOutputData[i] = reinterpret_cast<float *>(m_processBufferSharedMemory->data()) + i * (bufferSize * 2);
        }

        // Set processing information to VstHelper
        VstHelper::instance()->connectionStatus.isProcessing = true;
        VstHelper::instance()->connectionStatus.channelCount = channelCount;
        VstHelper::instance()->connectionStatus.bufferSize = bufferSize;
        VstHelper::instance()->connectionStatus.sampleRate = sampleRate;
        return true;
    }
    void VstPlaybackWorker::finalize() {
        qDebug() << "VstPlaybackWorker: finalize";
        m_planarOutputData.resize(0);
        m_bufferSwitchData = nullptr;
        m_processBufferSharedMemory->detach();
        VstHelper::instance()->connectionStatus.isProcessing = false;
    }

    bool VstPlaybackWorker::work(bool isRealtime, bool isPlaying, qint64 position, int bufferSize, int channelCount, float *const *output) {
        // TODO
        if(isPlaying) {
            for(int i = 0; i < channelCount; i++)
                genSineWave(position, bufferSize, 440, VstHelper::instance()->connectionStatus.sampleRate, output[i]);
            pos = 0;
        } else {
            for(int i = 0; i < channelCount; i++)
                genSineWave(pos, bufferSize, 880, VstHelper::instance()->connectionStatus.sampleRate, output[i]);
            pos += bufferSize;
        }
        return true;
    }
} // Internal