#include "ProjectCommonBlock.h"

#include "Managers/QsFileManager.h"
#include "Managers/WindowManager.h"

#include "Kernel/LvApplication.h"

#include "QMSystem.h"
#include "Utils/VirtualPathValidator.h"

#include "Serialization/QLVProject.h"

#include <QMessageBox>

static const char Slash = '/';

static const char LVPROJ_SUFFIX[] = ".lvproj";

static const char FLAG_OPEN[] = "%PROJ%";

int ProjectCommonBlock::newProjectIndex = 1;

ProjectCommonBlock::ProjectCommonBlock(QWidget *w) : w(w) {
}

ProjectCommonBlock::~ProjectCommonBlock() {
}

QString ProjectCommonBlock::newProject(const TemplateConfig &config) const {
    if (config.name.isEmpty() || !VirtualPathValidator::IsValidName(config.name)) {
        QMessageBox::critical(w, qApp->errorTitle(),
                              QObject::tr("Invalid project name!").arg(config.dir));
        return QString();
    }

    if (!Sys::isDirExist(config.dir)) {
        QMessageBox::critical(w, qApp->errorTitle(),
                              QObject::tr("Directory %1 dosn't exists!").arg(config.dir));
        return QString();
    }

    QString fileBase = config.name.chopped(config.name.endsWith(LVPROJ_SUFFIX, Qt::CaseInsensitive)
                                               ? QString(LVPROJ_SUFFIX).size()
                                               : 0);
    QString path = config.dir + Slash + fileBase;
    if (Sys::isDirExist(path)) {
        QMessageBox::critical(
            w, qApp->errorTitle(),
            QObject::tr("There's already a directory with same name on this path."));
        return QString();
    }
    if (!Sys::mkDir(path)) {
        QMessageBox::critical(w, qApp->errorTitle(),
                              QObject::tr("Failed to create project directory!"));
        return QString();
    }

    QString filePath = path + Slash + fileBase + LVPROJ_SUFFIX;
    LVModel::ProjectModel proj;
    proj.Name = config.name;

    // Add default speaker and default languages
    // QUESTION: shall we use translation in object names?
    // QUESTION: is it ok to keep dictionary files etc as "empty"?
    LVModel::SpeakerDefinition spk;
    spk.Id = proj.hexIdPublisher.Generate(4);
    spk.Name = QObject::tr("Default Speaker", "object name");
    proj.Speakers.push_back(spk);

    LVModel::LanguageDefinition lang;
    lang.Id = proj.hexIdPublisher.Generate(4);
    lang.Name = QObject::tr("Default Language", "object name");
    lang.AlignerRootPath = "";
    lang.DictionaryPath = "";
    lang.PhonemeSetPath = ""; 
    proj.Languages.push_back(lang);

    if (!proj.save(filePath)) {
        QMessageBox::critical(w, qApp->errorTitle(), QObject::tr("Failed to create project file!"));
        Sys::rmDir(path);
        return QString();
    }

    newProjectIndex++;

    return filePath;
}

void ProjectCommonBlock::openProject() const {
    QString path = QsFileManager::instance()->openFile(
        QObject::tr("Import"),
        QObject::tr("LabelVoice Files(*.lvproj);;All Files(%1)").arg(qApp->allFilesFilter()),
        FLAG_OPEN, w);
    if (!path.isEmpty()) {
        WindowManager::instance()->openProject(path);
    }
}

void ProjectCommonBlock::openProject(const QString &filename) const {
    WindowManager::instance()->openProject(filename);
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
