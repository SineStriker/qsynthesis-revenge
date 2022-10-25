#include "QLVProject.h"

#include "MathHelper.h"

#include "Utils/HexGenerator.h"
#include "Utils/VirtualPathValidator.h"

#include <QFile>
#include <QXmlStreamReader>

LVModel::LayerDefinition::LayerDefinition() {
    SubdivisionOf = 0;
    AlignedWith = 0;
    ValType = ValueType::Text;
}


LVModel::ItemResource::ItemResource(LVModel::ItemResource::ResourceType type) : Type(type) {
}

bool LVModel::ItemResource::operator<(const LVModel::ItemResource &other) const {
    if (VirtualPath < other.VirtualPath) {
        return true;
    }
    if (Type != other.Type) {
        return (Type == ResourceType::Placeholder) ? true : false;
    }
    return (Type == ResourceType::Definition && other.Type == ResourceType::Definition)
               ? (Name < other.Name)
               : false;
}

LVModel::ProjectModel::ProjectModel() {
}

LVModel::ProjectModel::~ProjectModel() {
}

bool LVModel::ProjectModel::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamReader reader(&file);
    bool success = true;
//    while (!reader.atEnd()) {
//        if (reader.hasError()) {
//            success = false;
//            break;
//        }
//        if (reader.isStartElement()) {
//            foreach (const QXmlStreamAttribute &attribute, reader.attributes()) {
//                qDebug() << attribute.name();
//                qDebug() << attribute.value();
//                qDebug() << endl;
//            }
//        }
//        reader.readNext();
//    }

    file.close();

    return success;
}

bool LVModel::ProjectModel::save(const QString &filename) {

    return true;
}

void LVModel::ProjectModel::reset() {
    Version.clear();
    Name.clear();
    LabelSchema.clear();
    Languages.clear();
    Speakers.clear();
    ItemResources.clear();
}

void LVModel::ProjectModel::Validate(bool reg) {
    ValidateLayers();
    ValidateLanguages(reg);
    ValidateSpeakers(reg);
    ValidateItemResources(reg);
}

void LVModel::ProjectModel::ValidateLayers() {
    for (int i = 0; i < LabelSchema.size(); ++i) {
        auto layer = LabelSchema[i];
        if (layer.SubdivisionOf >= i || layer.SubdivisionOf < -1) {
            layer.SubdivisionOf = -1;
        }

        if (layer.AlignedWith >= i || layer.AlignedWith < -1) {
            layer.AlignedWith = -1;
        }

        if (layer.SubdivisionOf == layer.AlignedWith) {
            layer.SubdivisionOf = -1;
        }
    }

    QList<int> buffer;
    for (int i = 0; i < LabelSchema.size(); ++i) {
        if (LabelSchema[i].AlignedWith <= 0) {
            continue;
        }

        int j = i;
        while (LabelSchema[j].SubdivisionOf >= LabelSchema[i].AlignedWith) {
            buffer.append(j);
            j = LabelSchema[j].SubdivisionOf;
        }

        if (j == LabelSchema[i].AlignedWith) {
            for (int idx : qAsConst(buffer)) {
                LabelSchema[idx].SubdivisionOf = -1;
                LabelSchema[idx].AlignedWith = LabelSchema[i].AlignedWith;
            }
        }
        buffer.clear();
    }
}

void LVModel::ProjectModel::ValidateLanguages(bool reg) {
    int i = 0;
    while (i < Languages.size()) {
        LanguageDefinition language = Languages[i];
        if (!HexGenerator::IsValidFormat(language.Id, 4) ||
            (reg && !HexGenerator::Register(language.Id))) {
            Languages.removeAt(i);
        } else {
            ++i;
        }
    }
}

void LVModel::ProjectModel::ValidateSpeakers(bool reg) {
    int i = 0;
    while (i < Speakers.size()) {
        SpeakerDefinition speaker = Speakers[i];
        if (!HexGenerator::IsValidFormat(speaker.Id, 4) ||
            (reg && !HexGenerator::Register(speaker.Id))) {
            Speakers.removeAt(i);
        } else {
            ++i;
        }
    }
}

void LVModel::ProjectModel::ValidateItemResources(bool reg) {
    // Validate IDs, paths and names.
    for (auto it = ItemResources.begin(); it != ItemResources.end(); ++it) {
        QString key = it.key();
        ItemResource item = it.value();

        bool speakerNotFound = true;
        for (const auto &spk : qAsConst(Speakers)) {
            if (spk.Id == item.Speaker) {
                speakerNotFound = false;
                break;
            }
        }

        bool languageNotFound = true;
        for (const auto &lang : qAsConst(Languages)) {
            if (lang.Id == item.Language) {
                languageNotFound = false;
                break;
            }
        }

        if (!HexGenerator::IsValidFormat(key, 8)                       /* invalid ID format */
            || speakerNotFound                                         /* speaker not found */
            || (!VirtualPathValidator ::IsValidPath(item.VirtualPath)) /* invalid path format */
            || (item.Type == ItemResource::Definition /* if this key represents an item */
                && (!VirtualPathValidator::IsValidName(item.Name) /* invalid name format */
                    || languageNotFound)) /* language not found */) {
            ItemResources.remove(key);
        }
    }

    auto resources = ItemResources.values();

    // Remove duplicating items and redundant placeholders.
    for (int i = 0; i < resources.size(); ++i) {
        ItemResource item = resources[i];
        switch (item.Type) {
            case ItemResource::Placeholder:
                if (i < resources.size() - 1 &&
                    resources[i + 1].VirtualPath.startsWith(item.VirtualPath)) {
                    ItemResources.remove(item.Id);
                }
                break;
            case ItemResource::Definition: {
                int j = i + 1;
                ItemResource other;
                while (j < resources.size() &&
                       (other = resources[j]).Type == ItemResource::Definition &&
                       other.VirtualPath == item.VirtualPath && other.Name == item.Name) {
                    ItemResources.remove(item.Id);
                    ++j;
                }
                i = j - 1;
                break;
            }
            default:
                break;
        }
    }

    if (!reg) {
        return;
    }

    // Register all remaining IDs.
    const auto &keys = ItemResources.keys();
    for (const QString &key : keys) {
        HexGenerator::Register(key);
    }
}
