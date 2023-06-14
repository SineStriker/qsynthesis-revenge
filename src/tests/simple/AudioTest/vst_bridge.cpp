//
// Created by Crs_1 on 2023/5/10.
//

#include <QThread>
#include <QApplication>
#include <QWindow>

#include "AudioTrack.h"
#include "Pcm32BitFloatAudioSource.h"
#include "VstAudioOutputManager.h"

extern "C" Q_DECL_EXPORT bool SingletonChecker() {
    return true;
}

AudioTrack *track;
Pcm32BitFloatAudioSource *src1;
Pcm32BitFloatAudioSource *src2;
VstAudioOutputManager *vstMgr;
QWindow *win;

class GuiThread: public QThread {
    Q_OBJECT
    void run() override {
        int argc = 1;
        const char *argv[1];
        argv[0] = "C:\\Users\\Crs_1\\qsynthesis-revenge\\out\\out-Windows-Debug\\bin\\AudioTest.dll";
        QApplication a(argc, (char**)argv);
        win = new QWindow;
        win->setFlag(Qt::WindowStaysOnTopHint);
        win->setFlag(Qt::WindowCloseButtonHint, false);
        win->setFlag(Qt::WindowTitleHint);
        connect(this, &GuiThread::showWindow, win, &QWindow::showNormal);
        connect(this, &GuiThread::hideWindow, win, &QWindow::hide);
        connect(this, &GuiThread::terminateApp, &a, &QApplication::quit);
        a.setQuitOnLastWindowClosed(false);
        a.exec();
    }
signals:
    void showWindow();
    void hideWindow();
    void terminateApp();
};

GuiThread *guiThread;

extern "C" Q_DECL_EXPORT bool Initializer() {
    guiThread = new GuiThread;
    guiThread->start();
    track = new AudioTrack(2, 48000);
    src1 = new Pcm32BitFloatAudioSource("D:\\Downloads\\test_source1.pcm", 2, 48000);
    src2 = new Pcm32BitFloatAudioSource("D:\\Downloads\\test_source2.pcm", 2, 48000);
    track->addSource(src1);
    track->addSource(src2);
    if(!track->open()) return false;
    track->setVolume(0.5);
    vstMgr = new VstAudioOutputManager;
    vstMgr->output(0)->addSource(track);
    return true;
}

extern "C" Q_DECL_EXPORT bool Terminator() {
    vstMgr->deleteLater();
    track->deleteLater();
    delete src1;
    delete src2;
    win->deleteLater();
    emit guiThread->terminateApp();
    guiThread->deleteLater();
    return true;
}

extern "C" Q_DECL_EXPORT void WindowOpener() {
    emit guiThread->showWindow();
}

extern "C" Q_DECL_EXPORT void WindowCloser() {
    emit guiThread->hideWindow();
}

extern "C" Q_DECL_EXPORT bool PlaybackProcessor(const VstAudioOutputManager::PlaybackParameters *playbackParameters, bool isPlaying, int32_t numOutputs, float *const *const *outputs) {
    return vstMgr->vstProcess(playbackParameters, isPlaying, numOutputs, outputs);
    return true;
}

extern "C" Q_DECL_EXPORT bool StateChangedCallback(qint64 size, const char *data) {
    return true;
}

extern "C" Q_DECL_EXPORT bool StateWillSaveCallback(qint64 &size, char *&data) {
    size = 0;
    return true;
}

extern "C" Q_DECL_EXPORT void StateSavedAsyncCallback(uint8_t* dataToFree) {
    return;
}

extern "C" Q_DECL_EXPORT void DirtySetterBinder(void (*setDirty)(bool)) {
    return;
}

extern "C" Q_DECL_EXPORT bool ProcessInitializer(bool isOffline, double sampleRate) {
    return vstMgr->vstProcessInitializer(isOffline, sampleRate);
}

#include "vst_bridge.moc"