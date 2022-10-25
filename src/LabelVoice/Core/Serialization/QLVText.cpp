#include "QLVText.h"

#include <QFile>
#include <QTextCodec>
#include <QTextStream>

#include <yaml-cpp/yaml.h>

#define PARSE_STR(NAME, KEY, CONF)                                                                 \
    NAME = QString::fromStdString(CONF[KEY].as<std::string>(NAME.toStdString()))
#define PARSE_DBL(NAME, KEY, CONF) NAME = CONF[KEY].as<double>(NAME)
#define PARSE_INT(NAME, KEY, CONF) NAME = CONF[KEY].as<int>(NAME)
#define PARSE_BOOL(NAME, KEY, CONF) NAME = CONF[KEY].as<bool>(NAME)

LVModel::AudioSourceReference::AudioSourceReference() {
    In = Out = 0;
}

LVModel::AudioSourceReference::~AudioSourceReference() {
}

LVModel::Boundary::Boundary() {
    Id = 0;
    Position = 0;
}

LVModel::Boundary::~Boundary() {
}

LVModel::TextLabelModel::TextLabelModel() {
    Version = 1;
    _maxId = 0;
}

LVModel::TextLabelModel::~TextLabelModel() {
}

bool LVModel::TextLabelModel::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream in(&file); // UTF-8 with BOM
    QString data = in.readAll();

    file.close();

    try {
        YAML::Node config = YAML::Load(data.toStdString());

        // Version
        PARSE_INT(Version, "version", config);

        // Audio
        YAML::Node audio_config = config["audio"];
        PARSE_STR(AudioSource.Path, "path", audio_config);
        PARSE_DBL(AudioSource.In, "in", audio_config);
        PARSE_DBL(AudioSource.Out, "out", audio_config);

        // Layers
        YAML::Node layers_config = config["layers"];
        for (auto it = layers_config.begin(); it != layers_config.end(); ++it) {
            YAML::Node layer_config = it->second;

            TextLayer layer;

            // Name
            PARSE_STR(layer.Name, "name", layer_config);

            // Boundaries
            YAML::Node bounds_config = layer_config["boundaries"];
            for (auto it2 = bounds_config.begin(); it2 != bounds_config.end(); ++it2) {
                YAML::Node bound_config = it2->second;

                Boundary boundary;

                PARSE_INT(boundary.Id, "id", bound_config);
                PARSE_INT(boundary.Position, "position", bound_config);
                PARSE_STR(boundary.Text, "text", bound_config);

                layer.Boundaries.insert(boundary);
            }

            Layers.append(layer);
        }

        // Groups
        YAML::Node groups_config = config["groups"];
        for (auto it = groups_config.begin(); it != groups_config.end(); ++it) {
            YAML::Node group_config = it->second;

            std::vector<int> arr = group_config.as<std::vector<int>>();
            std::set<int> set;
            for (int i : qAsConst(arr)) {
                set.insert(i);
            }

            BindingGroups.append(set);
        }

    } catch (...) {
        return false;
    }

    return true;
}

bool LVModel::TextLabelModel::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }

    YAML::Emitter root;
    {
        root << YAML::BeginMap;

        // Version
        root << YAML::Key << "version" << YAML::Value << Version;

        // Audio
        root << YAML::Key << "audio" << YAML::Value;
        {
            root << YAML::BeginMap;

            // Path
            root << YAML::Key << "path" << YAML::Value << AudioSource.Path.toStdString();

            // In
            root << YAML::Key << "in" << YAML::Value << AudioSource.In;

            // Out
            root << YAML::Key << "out" << YAML::Value << AudioSource.Out;

            root << YAML::EndMap;
        }

        // Layers
        root << YAML::Key << "layers" << YAML::Value;
        root << YAML::BeginSeq;

        for (const TextLayer &layer : qAsConst(Layers)) {
            // Layer
            root << YAML::BeginMap;
            {
                // Name
                root << YAML::Key << "name" << YAML::Value << layer.Name.toStdString();

                // Boundaries
                root << YAML::Key << "boundaries" << YAML::Value;
                root << YAML::BeginSeq;

                for (const Boundary &bound : layer.Boundaries) {
                    // Boundary
                    root << YAML::BeginMap;
                    {
                        // Id
                        root << YAML::Key << "id" << YAML::Value << bound.Id;

                        // Position
                        root << YAML::Key << "position" << YAML::Value << bound.Position;

                        // Text
                        root << YAML::Key << "text" << YAML::Value << bound.Text.toStdString();
                    }
                    root << YAML::EndMap;
                }
                root << YAML::EndSeq;
            }
            root << YAML::EndMap;
        }
        root << YAML::EndSeq;

        // Groups
        root << YAML::Key << "groups" << YAML::Value;

        root << YAML::BeginSeq;
        for (const auto &set : qAsConst(BindingGroups)) {
            // Group
            root << YAML::BeginSeq;
            for (int i : qAsConst(set)) {
                root << i;
            }
            root << YAML::EndSeq;
        }
        root << YAML::EndSeq;

        root << YAML::EndMap;
    }

    QTextStream out(&file); // UTF-8 with BOM
    out.setCodec(QTextCodec::codecForName("UTF-8"));

    out << QString::fromStdString(std::string(root.c_str(), root.size()));

    file.close();

    return true;
}

void LVModel::TextLabelModel::reset() {
    AudioSource = AudioSourceReference();
    Layers.clear();
    BindingGroups.clear();
}

int LVModel::TextLabelModel::maxIdReached() const {
    return _maxId;
}

void LVModel::TextLabelModel::Validate() {
    if (AudioSource.In < 0) {
        AudioSource.In = 0;
    }

    if (AudioSource.Out < 0) {
        AudioSource.Out = 0;
    }

    if (AudioSource.In > AudioSource.Out) {
        qSwap(AudioSource.In, AudioSource.Out);
    }

    _ids.clear();
    _maxId = 0;

    for (auto &layer : Layers) {
        for (auto it = layer.Boundaries.begin(); it != layer.Boundaries.end(); ++it) {
            auto id = it->Id;
            if (_ids.contains(id) || id < 0) {
                it = layer.Boundaries.erase(it);
            } else {
                _ids.insert(id);
                if (id > _maxId) {
                    _maxId = id;
                }
            }
        }
    }

    for (auto &group : BindingGroups) {
        for (auto it = group.begin(); it != group.end(); ++it) {
            if (!_ids.contains(*it)) {
                it = group.erase(it);
            }
        }
    }

    for (auto it = BindingGroups.begin(); it != BindingGroups.end(); ++it) {
        if (it->size() == 0) {
            it = BindingGroups.erase(it);
        }
    }
}
