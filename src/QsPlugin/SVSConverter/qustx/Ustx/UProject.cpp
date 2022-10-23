#include "UProject.h"
#include "Util/MusicMath.h"

#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <yaml-cpp/yaml.h>

#define PARSE_STR(NAME, KEY, CONF)                                                                 \
    NAME = QString::fromStdString(CONF[KEY].as<std::string>(NAME.toStdString()))
#define PARSE_DBL(NAME, KEY, CONF) NAME = CONF[KEY].as<double>(NAME)
#define PARSE_INT(NAME, KEY, CONF) NAME = CONF[KEY].as<int>(NAME)
#define PARSE_BOOL(NAME, KEY, CONF) NAME = CONF[KEY].as<bool>(NAME)

#define PARSE_STR_CONV(NAME, KEY, CONF, CONV)                                                      \
    NAME = CONV(QString::fromStdString(CONF[KEY].as<std::string>(std::string())))

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

        // Root scalars
        PARSE_STR(name, "name", config);
        PARSE_STR(comment, "comment", config);
        PARSE_STR(outputDir, "output_dir", config);
        PARSE_STR(cacheDir, "cache_dir", config);
        PARSE_STR(ustxVersion, "ustx_version", config);

        PARSE_DBL(bpm, "bpm", config);
        PARSE_INT(beatPerBar, "beat_per_bar", config);
        PARSE_INT(beatUnit, "beat_unit", config);
        PARSE_INT(resolution, "resolution", config);

        // Expressions
        YAML::Node exp_config = config["expressions"];
        for (auto it = exp_config.begin(); it != exp_config.end(); ++it) {
            YAML::Node c = it->second;

            UExpressionDescriptor desc;
            PARSE_STR(desc.name, "name", c);
            PARSE_STR(desc.abbr, "abbr", c);
            PARSE_STR_CONV(desc.type, "type", c, UExpressionDescriptor::StringtoType);
            PARSE_DBL(desc.min, "min", c);
            PARSE_DBL(desc.max, "max", c);
            PARSE_DBL(desc.defaultValue, "default_value", c);
            PARSE_BOOL(desc.isFlag, "is_flag", c);
            PARSE_STR(desc.flag, "flag", c);

            expressions.insert(QString::fromStdString(it->first.Scalar()), desc);
        }

        // Tracks
        YAML::Node tracks_config = config["tracks"];
        for (auto it = exp_config.begin(); it != exp_config.end(); ++it) {
            YAML::Node c = it->second;

            UTrack track;
            PARSE_STR(track.singer, "singer", c);
            PARSE_STR(track.phonemizer, "phonemizer", c);
            PARSE_STR(track.renderer, "renderer", c);
            PARSE_BOOL(track.mute, "mute", c);
            PARSE_BOOL(track.solo, "solo", c);
            PARSE_DBL(track.volume, "volume", c);

            tracks.append(track);
        }

        // Voice Parts
        YAML::Node vp_config = config["voice_parts"];
        for (auto it = vp_config.begin(); it != vp_config.end(); ++it) {
            YAML::Node c = it->second;

            UVoicePart voicePart;
            PARSE_STR(voicePart.name, "name", c);
            PARSE_STR(voicePart.comment, "comment", c);
            PARSE_INT(voicePart.trackNo, "track_no", c);
            PARSE_INT(voicePart.position, "position", c);

            // Notes
            YAML::Node notes_config = c["notes"];
            for (auto it2 = notes_config.begin(); it2 != notes_config.end(); ++it2) {
                YAML::Node c2 = it2->second;

                UNote note;
                PARSE_INT(note.position, "position", c2);
                PARSE_INT(note.duration, "duration", c2);
                PARSE_INT(note.tone, "tone", c2);
                PARSE_STR(note.lyric, "lyric", c2);

                // Pitch
                YAML::Node pitch_config = c2["pitch"];
                YAML::Node data_config = pitch_config["data"];
                for (auto it3 = data_config.begin(); it3 != data_config.end(); ++it3) {
                    YAML::Node c3 = it3->second;

                    PitchPoint p;
                    PARSE_DBL(p.X, "x", c3);
                    PARSE_DBL(p.Y, "y", c3);
                    PARSE_STR_CONV(p.shape, "shape", c3, PitchPoint::StringToShape);

                    note.pitch.data.append(p);
                }
                PARSE_BOOL(note.pitch.snapFirst, "snap_first", pitch_config);

                // Vibrato
                YAML::Node vbr_config = c2["vibrato"];
                note.vibrato.setLength(vbr_config["length"].as<double>(note.vibrato.length()));
                note.vibrato.setPeriod(vbr_config["period"].as<double>(note.vibrato.period()));
                note.vibrato.setDepth(vbr_config["depth"].as<double>(note.vibrato.depth()));
                note.vibrato.setIn(vbr_config["in"].as<double>(note.vibrato.in()));
                note.vibrato.setOut(vbr_config["out"].as<double>(note.vibrato.out()));
                note.vibrato.setShift(vbr_config["shift"].as<double>(note.vibrato.shift()));
                note.vibrato.setDrift(vbr_config["drift"].as<double>(note.vibrato.drift()));

                voicePart.notes.append(note);
            }

            // Curves

            voiceParts.append(voicePart);
        }


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

int UProject::MillisecondToTick(double ms) const {
    return MusicMath::MillisecondToTick(ms, bpm, beatUnit, resolution);
}

double UProject::TickToMillisecond(double tick) const {
    return MusicMath::TickToMillisecond(tick, bpm, beatUnit, resolution);
}
