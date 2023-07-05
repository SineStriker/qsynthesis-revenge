#ifndef VSTGLOBAL_IVSTBRIDGE_H
#define VSTGLOBAL_IVSTBRIDGE_H

#include "IEditorCallbacks.h"

class IVstBridge {

public:

    virtual bool initialize() = 0;
    virtual void terminate() = 0;

    virtual void bindEditorCallbacks(IEditorCallbacks *iEditorCallbacks) = 0;

    virtual void showWindow() = 0;
    virtual void hideWindow() = 0;

    virtual bool initializeProcess(int channelCount, int maxBufferSize, double sampleRate) = 0;
    virtual bool processPlayback(
        bool isRealtime,
        bool isPlaying,
        long long position,
        int size,
        int channelCount,
        float *const *outputs) = 0;
    virtual void finalizeProcess() = 0;

    virtual void stateChanged(int size, const char *data) = 0;
    virtual bool stateWillSave(int &size, const char *& data) = 0;
    virtual void stateSaved(const char *data) = 0;

};

#endif // VSTGLOBAL_IVSTBRIDGE_H
