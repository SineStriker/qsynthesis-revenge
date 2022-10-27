#include "ProjectCommonBlock.h"

#include "Managers/FileManager.h"
#include "Managers/WindowManager.h"

#include "Kernel/LvApplication.h"

#include <QMessageBox>

static const char FLAG_OPEN[] = "%PROJ%";

ProjectCommonBlock::ProjectCommonBlock(QWidget *w) : w(w) {
}

ProjectCommonBlock::~ProjectCommonBlock() {
}

void ProjectCommonBlock::newProject() const {
    WindowManager::instance()->newProject();
}

void ProjectCommonBlock::openProject() const {
    QString path = FileManager::instance()->openFile(
        QObject::tr("Import"),
        QObject::tr("LabelVoice Files(*.lvproj);;All Files(%1)").arg(qApp->allFilesFilter()),
        FLAG_OPEN, w);
    if (!path.isEmpty()) {
        WindowManager::instance()->openProject(path);
    }
}

void ProjectCommonBlock::aboutApp() const {
    //            auto ffmpeg = FF::GetFFmpegInfo();
    //            auto sdl = FF::GetSDLInfo();

    //            QString ffmpegStr;
    //            for (auto it = ffmpeg.begin(); it != ffmpeg.end(); ++it) {
    //                ffmpegStr += tr("<p>%1 %2</p>")
    //                                 .arg(it->name, QString::asprintf("%d.%d.%d",
    //                                 it->version[0],
    //                                                                  it->version[1],
    //                                                                  it->version[2]));
    //            }
    //            ffmpegStr += tr("<p>License: %3</p>"
    //                            "<p>Configuration: %4</p>")
    //                             .arg(ffmpeg.front().license,
    //                             ffmpeg.front().configuration);

    //            QString sdlStr = tr("<p>%1 %2</p>")
    //                                 .arg("SDL2", QString::asprintf("%d.%d.%d",
    //                                 sdl.version[0],
    //                                                                sdl.version[1],
    //                                                                sdl.version[2]));

    QString text =
       QObject::tr("<h2>%1 %2</h2>"
           "<p>"
           "<span style=\"font-weight: bold;\">LabelVoice</span> with innovative "
           "framework.</p>"
           "<p>Copyright (C) OpenVPI Team, 2020-2022. </p>"
           /*"<h3>FFmpeg</h3>"
           "%3"
           "<h3>SDL2</h3>"
           "%4"*/)
            .arg(qApp->applicationName(), qApp->applicationVersion()/*, ffmpegStr, sdlStr*/);
    QMessageBox::about(w, QObject::tr("About %1").arg(qApp->applicationName()), text);
}

void ProjectCommonBlock::aboutQt() const {
    QMessageBox::aboutQt(w, QObject::tr("About Qt"));
}
