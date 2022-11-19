#include <QCoreApplication>

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QPluginLoader>
#include <QTextStream>

#include "Api/IAudioDecoder.h"
#include "Api/IAudioPlayback.h"

#include "Common/CodecArguments.h"
#include "Common/SampleFormat.h"

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

#define PLUGINS_DIR qApp->applicationDirPath() + "/" + TEST_PLUGINS_DIR + "/plugins"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    if (argc < 2) {
        qDebug() << "Usage: DecodeTest [audio file]";
        return 0;
    }

    QString path(argv[1]);
    if (!QFileInfo(path).isFile()) {
        qDebug() << QString("File %1 doesn't exist.");
        return -1;
    }

    // Set library loading info
#ifdef Q_OS_WINDOWS
    ::SetDllDirectoryW(
        QString(qApp->applicationDirPath() + "/" + TEST_LIB_DIR).toStdWString().c_str());
#endif
    qApp->addLibraryPath(PLUGINS_DIR);


    auto decoder = qobject_cast<IAudioDecoder *> //
        (QPluginLoader("audiodecoders/FFmpegDecoder").instance());
    auto playback = qobject_cast<IAudioPlayback *> //
        (QPluginLoader("audioplaybacks/SDLPlayback").instance());

    if (!decoder || !playback) {
        qDebug() << "Failed to load plugins.";
        return -1;
    }

    qDebug() << "Audio File:" << QFileInfo(path).fileName();

    if (!decoder->open({{QsMedia::KEY_NAME_FILE_NAME, path},
                        {QsMedia::KEY_NAME_SAMPLE_RATE, 44100},
                        {QsMedia::KEY_NAME_SAMPLE_FORMAT, QsMedia::AV_SAMPLE_FMT_FLT},
                        {QsMedia::KEY_NAME_CHANNELS, 2}})) {
        qDebug() << "Failed to initialize decoder.";
        return -1;
    }

    if (!playback->setup(IAudioPlayback::PlaybackArguments{1024, 44100, 2, {}})) {
        qDebug() << "Failed to initialize playback.";
        return -1;
    }

    playback->setDecoder(decoder);
    playback->play();

    return a.exec();
}
