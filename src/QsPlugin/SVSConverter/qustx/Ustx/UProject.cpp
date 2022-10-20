#include "UProject.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <yaml-cpp/yaml.h>

UProject::UProject() {
    reset();
}

UProject::~UProject() {
}

bool UProject::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream in(&file); // UTF-8 with BOM
    QString data = in.readAll();

    file.close();

    try {
        YAML::Node config = YAML::Load(data.toStdString());

        name = QString::fromStdString(config["name"].as<std::string>());
        comment = QString::fromStdString(config["comment"].as<std::string>());
        outputDir = QString::fromStdString(config["output_dir"].as<std::string>());
        cacheDir = QString::fromStdString(config["cache_dir"].as<std::string>());
        ustxVersion = QString::fromStdString(config["ustx_version"].as<std::string>());

        bpm = config["bpm"].as<double>();
        beatPerBar = config["beat_per_bar"].as<int>();
        beatUnit = config["beat_unit"].as<int>();
        resolution = config["resolution"].as<int>();

        qDebug() << "name" << name;
        qDebug() << "comment" << comment;
        qDebug() << "outputDir" << outputDir;
        qDebug() << "cacheDir" << cacheDir;
        qDebug() << "ustxVersion" << ustxVersion;

        qDebug() << "bpm" << bpm;
        qDebug() << "beatPerBar" << beatPerBar;
        qDebug() << "beatUnit" << beatUnit;
        qDebug() << "resolution" << resolution;

    } catch (...) {
        return false;
    }

    return true;
}

bool UProject::save(const QString &filename) {
    return false;
}

void UProject::reset() {
    name = "New Project";
    comment.clear();
    outputDir = "Vocal";
    cacheDir = "UCache";
    ustxVersion.clear();

    bpm = 120;
    beatPerBar = 4;
    beatUnit = 4;
    resolution = 480;

    expressions.clear();
    tracks.clear();
    voiceParts.clear();
    waveParts.clear();
}
