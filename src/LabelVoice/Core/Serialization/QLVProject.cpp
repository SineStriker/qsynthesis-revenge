#include "QLVProject.h"

#include "MathHelper.h"

#include "Utils/HexGenerator.h"
#include "Utils/VirtualPathValidator.h"

#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>

#include <list>

static const char LVPROJ_ROOT[] = "LVProject";
static const char LVPROJ_KEY_VERSION[] = "Version";
static const char LVPROJ_KEY_NAME[] = "Name";

static const char LVPROJ_SECTION_LABEL_SCHEMA[] = "LabelSchema";
static const char LVPROJ_KEY_LAYER[] = "Layer";
static const char LVPROJ_ATTRIBUTE_LAYER_CATEGORY[] = "Category";
static const char LVPROJ_ATTRIBUTE_LAYER_NAME[] = "Name";
static const char LVPROJ_ATTRIBUTE_LAYER_SUBVISION_OF[] = "SubdivisionOf";
static const char LVPROJ_ATTRIBUTE_LAYER_ALIGNED_WITH[] = "AlignedWith";
static const char LVPROJ_ATTRIBUTE_LAYER_VALUE_TYPE[] = "ValueType";

static const char LVPROJ_SECTION_LANGUAGES[] = "Languages";
static const char LVPROJ_KEY_LANGUAGE[] = "Language";
static const char LVPROJ_ATTRIBUTE_LANGUAGE_ID[] = "Id";
static const char LVPROJ_ATTRIBUTE_LANGUAGE_NAME[] = "Name";
static const char LVPROJ_ATTRIBUTE_LANGUAGE_DICTIONARY[] = "Dictionary";
static const char LVPROJ_ATTRIBUTE_LANGUAGE_PHONEME_SET[] = "PhonemeSet";
static const char LVPROJ_ATTRIBUTE_LANGUAGE_ALIGNER[] = "Aligner";

static const char LVPROJ_SECTION_SPEAKERS[] = "Speakers";
static const char LVPROJ_KEY_SPEAKER[] = "Speaker";
static const char LVPROJ_ATTRIBUTE_SPEAKER_ID[] = "Id";
static const char LVPROJ_ATTRIBUTE_SPEAKER_NAME[] = "Name";

static const char LVPROJ_SECTION_ITEM_RESOURCES[] = "ItemResources";
static const char LVPROJ_KEY_ITEM[] = "Item";
static const char LVPROJ_ATTRIBUTE_ITEM_ID[] = "Id";
static const char LVPROJ_ATTRIBUTE_ITEM_NAME[] = "Name";
static const char LVPROJ_ATTRIBUTE_ITEM_SPEAKER[] = "Speaker";
static const char LVPROJ_ATTRIBUTE_ITEM_LANGUAGE[] = "Language";
static const char LVPROJ_ATTRIBUTE_ITEM_VIRTUAL_PATH[] = "VirtualPath";
static const char LVPROJ_KEY_PLACEHOLDER[] = "Placeholder";

QString LVModel::LayerCategoryToString(LayerCategory c) {
    QString res;
    switch (c) {
        case LayerCategory::Sentence:
            res = "Sentence";
            break;
        case LayerCategory::Grapheme:
            res = "Grapheme";
            break;
        case LayerCategory::Phoneme:
            res = "Phoneme";
            break;
        case LayerCategory::Pitch:
            res = "Pitch";
            break;
        case LayerCategory::Duration:
            res = "Duration";
            break;
        case LayerCategory::Custom:
            res = "Custom";
            break;
    }
    return res;
}

LVModel::LayerCategory LVModel::StringToLayerCategory(const QString &s) {
    LayerCategory res = LayerCategory::Custom;
    if (s == "Sentence") {
        res = LayerCategory::Sentence;
    } else if (s == "Grapheme") {
        res = LayerCategory::Grapheme;
    } else if (s == "Phoneme") {
        res = LayerCategory::Phoneme;
    } else if (s == "Pitch") {
        res = LayerCategory::Pitch;
    } else if (s == "Duration") {
        res = LayerCategory::Duration;
    }
    return res;
}

QString LVModel::ValueTypeToString(ValueType c) {
    QString res;
    switch (c) {
        case ValueType::Text:
            res = "Text";
            break;
        case ValueType::Integer:
            res = "Integer";
            break;
        case ValueType::Float:
            res = "Float";
            break;
        case ValueType::Pitch:
            res = "Pitch";
            break;
    }
    return res;
}

LVModel::ValueType LVModel::StringToValueType(const QString &s) {
    ValueType res = ValueType::Text;
    if (s == "Integer") {
        res = ValueType::Integer;
    } else if (s == "Float") {
        res = ValueType::Float;
    } else if (s == "Pitch") {
        res = ValueType::Pitch;
    }
    return res;
}


LVModel::LayerDefinition::LayerDefinition() {
    SubdivisionOf = 0;
    AlignedWith = 0;
    ValType = ValueType::Text;
}

LVModel::LayerDefinition::~LayerDefinition() {
}

LVModel::ItemResource::ItemResource(LVModel::ItemResource::ResourceType type) : Type(type) {
}

LVModel::ItemResource::~ItemResource() {
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
    reset();
}

LVModel::ProjectModel::~ProjectModel() {
}

bool LVModel::ProjectModel::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamReader reader(&file);
    bool success = true;

    enum class Sections {
        Single,
        Outer,
        LVProject,
        LabelSchema,
        Languages,
        Speakers,
        ItemResources,
    };

    std::list<Sections> sections;
    sections.push_back(Sections::Outer);

    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType nType = reader.readNext();
        switch (nType) {
            case QXmlStreamReader::StartDocument: {
                qDebug() << "********** Start LVPROJ ********** ";
                QString strVersion = reader.documentVersion().toString();
                QString strEncoding = reader.documentEncoding().toString();
                bool bAlone = reader.isStandaloneDocument();
                qDebug() << QString("Version: %1  Encoding: %2  Standalone: %3")
                                .arg(strVersion, strEncoding, QString::number(bAlone));
                break;
            }
            case QXmlStreamReader::StartElement: {
                QString name = reader.name().toString();
                switch (sections.back()) {
                    case Sections::Outer:
                        if (name == LVPROJ_ROOT) {
                            // Enter LVProject
                            sections.push_back(Sections::LVProject);
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::LVProject:
                        if (name == LVPROJ_KEY_VERSION) {
                            // Parse LVProject.Version
                            reader.readNext();
                            if (reader.isCharacters()) {
                                Version = reader.text().toString();
                            } else {
                                success = false;
                            }
                            sections.push_back(Sections::Single);
                        } else if (name == LVPROJ_KEY_NAME) {
                            // Parse LVProject.Name
                            reader.readNext();
                            if (reader.isCharacters()) {
                                Name = reader.text().toString();
                            } else {
                                success = false;
                            }
                            sections.push_back(Sections::Single);
                        } else if (name == LVPROJ_SECTION_LABEL_SCHEMA) {
                            // Enter LVProject.LabelSchema
                            sections.push_back(Sections::LabelSchema);
                        } else if (name == LVPROJ_SECTION_LANGUAGES) {
                            // Enter LVProject.Languages
                            sections.push_back(Sections::Languages);
                        } else if (name == LVPROJ_SECTION_SPEAKERS) {
                            // Enter LVProject.Speakers
                            sections.push_back(Sections::Speakers);
                        } else if (name == LVPROJ_SECTION_ITEM_RESOURCES) {
                            // Enter LVProject.ItemResources
                            sections.push_back(Sections::ItemResources);
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::LabelSchema:
                        if (name == LVPROJ_KEY_LAYER) {
                            // Parse LVProject.LabelSchema.Layer
                            LayerDefinition layer;
                            QXmlStreamAttributes attrs = reader.attributes();
                            for (const auto &attr : qAsConst(attrs)) {
                                QString name = attr.name().toString();
                                QString val = attr.value().toString();
                                if (name == LVPROJ_ATTRIBUTE_LAYER_CATEGORY) {
                                    layer.Category = StringToLayerCategory(val);
                                } else if (name == LVPROJ_ATTRIBUTE_LAYER_NAME) {
                                    layer.Name = val;
                                } else if (name == LVPROJ_ATTRIBUTE_LAYER_SUBVISION_OF) {
                                    layer.SubdivisionOf = val.toInt();
                                } else if (name == LVPROJ_ATTRIBUTE_LAYER_ALIGNED_WITH) {
                                    layer.AlignedWith = val.toInt();
                                } else if (name == LVPROJ_ATTRIBUTE_LAYER_VALUE_TYPE) {
                                    layer.ValType = StringToValueType(val);
                                }
                            }
                            LabelSchema.push_back(layer);
                            sections.push_back(Sections::Single);
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::Languages:
                        if (name == LVPROJ_KEY_LANGUAGE) {
                            // Parse LVProject.Languages.Language
                            LanguageDefinition lang;
                            QXmlStreamAttributes attrs = reader.attributes();
                            for (const auto &attr : qAsConst(attrs)) {
                                QString name = attr.name().toString();
                                QString val = attr.value().toString();
                                if (name == LVPROJ_ATTRIBUTE_LANGUAGE_ID) {
                                    lang.Id = val;
                                } else if (name == LVPROJ_ATTRIBUTE_LANGUAGE_NAME) {
                                    lang.Name = val;
                                } else if (name == LVPROJ_ATTRIBUTE_LANGUAGE_DICTIONARY) {
                                    lang.DictionaryPath = val;
                                } else if (name == LVPROJ_ATTRIBUTE_LANGUAGE_PHONEME_SET) {
                                    lang.PhonemeSetPath = val;
                                } else if (name == LVPROJ_ATTRIBUTE_LANGUAGE_ALIGNER) {
                                    lang.AlignerRootPath = val;
                                }
                            }
                            Languages.push_back(lang);
                            sections.push_back(Sections::Single);
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::Speakers:
                        if (name == LVPROJ_KEY_SPEAKER) {
                            // Parse LVProject.Languages.Speaker
                            SpeakerDefinition speaker;
                            QXmlStreamAttributes attrs = reader.attributes();
                            for (const auto &attr : qAsConst(attrs)) {
                                QString name = attr.name().toString();
                                QString val = attr.value().toString();
                                if (name == LVPROJ_ATTRIBUTE_SPEAKER_ID) {
                                    speaker.Id = val;
                                } else if (name == LVPROJ_ATTRIBUTE_SPEAKER_NAME) {
                                    speaker.Name = val;
                                }
                            }
                            Speakers.push_back(speaker);
                            sections.push_back(Sections::Single);
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::ItemResources:
                        if (name == LVPROJ_KEY_ITEM) {
                            // Parse LVProject.Languages.Item
                            ItemResource item(ItemResource::Definition);
                            QXmlStreamAttributes attrs = reader.attributes();
                            for (const auto &attr : qAsConst(attrs)) {
                                QString name = attr.name().toString();
                                QString val = attr.value().toString();
                                if (name == LVPROJ_ATTRIBUTE_ITEM_ID) {
                                    item.Id = val;
                                } else if (name == LVPROJ_ATTRIBUTE_ITEM_NAME) {
                                    item.Name = val;
                                } else if (name == LVPROJ_ATTRIBUTE_ITEM_SPEAKER) {
                                    item.Speaker = val;
                                } else if (name == LVPROJ_ATTRIBUTE_ITEM_LANGUAGE) {
                                    item.Language = val;
                                } else if (name == LVPROJ_ATTRIBUTE_ITEM_VIRTUAL_PATH) {
                                    item.VirtualPath = val;
                                }
                            }
                            ItemResources.insert(item.Id, item);
                            sections.push_back(Sections::Single);
                        } else if (name == LVPROJ_KEY_PLACEHOLDER) {
                            // Parse LVProject.Languages.Placeholder
                            ItemResource placeholder;
                            QXmlStreamAttributes attrs = reader.attributes();
                            for (const auto &attr : qAsConst(attrs)) {
                                QString name = attr.name().toString();
                                QString val = attr.value().toString();
                                if (name == LVPROJ_ATTRIBUTE_ITEM_ID) {
                                    placeholder.Id = val;
                                } else if (name == LVPROJ_ATTRIBUTE_ITEM_SPEAKER) {
                                    placeholder.Speaker = val;
                                } else if (name == LVPROJ_ATTRIBUTE_ITEM_VIRTUAL_PATH) {
                                    placeholder.VirtualPath = val;
                                }
                            }
                            ItemResources.insert(placeholder.Id, placeholder);
                            sections.push_back(Sections::Single);
                        } else {
                            success = false;
                        }
                        break;
                    default:
                        success = false;
                        break;
                }
                break;
            }
            case QXmlStreamReader::EndElement: {
                QString name = reader.name().toString();
                switch (sections.back()) {
                    case Sections::Outer:
                        success = false;
                        break;
                    case Sections::LVProject:
                        if (name == LVPROJ_ROOT) {
                            // Exit LVProject
                            sections.pop_back();
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::LabelSchema:
                        if (name == LVPROJ_SECTION_LABEL_SCHEMA) {
                            // Exit LVProject.LabelSchema
                            sections.pop_back();
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::Languages:
                        if (name == LVPROJ_SECTION_LANGUAGES) {
                            // Exit LVProject.Languages
                            sections.pop_back();
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::Speakers:
                        if (name == LVPROJ_SECTION_SPEAKERS) {
                            // Exit LVProject.Speakers
                            sections.pop_back();
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::ItemResources:
                        if (name == LVPROJ_SECTION_ITEM_RESOURCES) {
                            // Exit LVProject.ItemResources
                            sections.pop_back();
                        } else {
                            success = false;
                        }
                        break;
                    default:
                        // Exit SingleItem
                        sections.pop_back();
                        break;
                }
                break;
            }
            case QXmlStreamReader::EndDocument: {
                qDebug() << QString::fromLocal8Bit("********** End LVPROJ ********** ");
                break;
            }
            default:
                break;
        }
        if (!success) {
            break;
        } else if (reader.hasError()) {
            success = false;
            break;
        }
    }

    file.close();

    if (success) {
        Validate(true);
    }

    return success;
}

bool LVModel::ProjectModel::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }

    Validate();

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    {
        // LVProject
        writer.writeStartElement(LVPROJ_ROOT);
        {
            // LVProject.Version
            writer.writeStartElement(LVPROJ_KEY_VERSION);
            writer.writeCharacters(Version);
            writer.writeEndElement();

            // LVProject.Name
            writer.writeStartElement(LVPROJ_KEY_NAME);
            writer.writeCharacters(Name);
            writer.writeEndElement();

            // LVProject.LabelSchema
            writer.writeStartElement(LVPROJ_SECTION_LABEL_SCHEMA);
            for (const LayerDefinition &layer : qAsConst(LabelSchema)) {
                writer.writeStartElement(LVPROJ_KEY_LAYER);
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LAYER_CATEGORY,
                                      LayerCategoryToString(layer.Category));
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LAYER_NAME, layer.Name);
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LAYER_SUBVISION_OF,
                                      QString::number(layer.SubdivisionOf));
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LAYER_ALIGNED_WITH,
                                      QString::number(layer.AlignedWith));
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LAYER_VALUE_TYPE,
                                      ValueTypeToString(layer.ValType));
                writer.writeEndElement();
            }
            writer.writeEndElement();

            // LVProject.Languages
            writer.writeStartElement(LVPROJ_SECTION_LANGUAGES);
            for (const LanguageDefinition &lang : qAsConst(Languages)) {
                writer.writeStartElement(LVPROJ_KEY_LANGUAGE);
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LANGUAGE_ID, lang.Id);
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LANGUAGE_NAME, lang.Name);
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LANGUAGE_DICTIONARY, lang.DictionaryPath);
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LANGUAGE_PHONEME_SET, lang.PhonemeSetPath);
                writer.writeAttribute(LVPROJ_ATTRIBUTE_LAYER_ALIGNED_WITH, lang.AlignerRootPath);
                writer.writeEndElement();
            }
            writer.writeEndElement();

            // LVProject.Speakers
            writer.writeStartElement(LVPROJ_SECTION_SPEAKERS);
            for (const SpeakerDefinition &speaker : qAsConst(Speakers)) {
                writer.writeStartElement(LVPROJ_KEY_SPEAKER);
                writer.writeAttribute(LVPROJ_ATTRIBUTE_SPEAKER_ID, speaker.Id);
                writer.writeAttribute(LVPROJ_ATTRIBUTE_SPEAKER_NAME, speaker.Name);
                writer.writeEndElement();
            }
            writer.writeEndElement();

            // LVProject.ItemResources
            writer.writeStartElement(LVPROJ_SECTION_ITEM_RESOURCES);
            for (const ItemResource &item : qAsConst(ItemResources)) {
                if (item.Type == ItemResource::Definition) {
                    writer.writeStartElement(LVPROJ_KEY_ITEM);
                    writer.writeAttribute(LVPROJ_ATTRIBUTE_ITEM_ID, item.Id);
                    writer.writeAttribute(LVPROJ_ATTRIBUTE_ITEM_NAME, item.Name);
                    writer.writeAttribute(LVPROJ_ATTRIBUTE_ITEM_SPEAKER, item.Speaker);
                    writer.writeAttribute(LVPROJ_ATTRIBUTE_ITEM_LANGUAGE, item.Language);
                    writer.writeAttribute(LVPROJ_ATTRIBUTE_ITEM_VIRTUAL_PATH, item.VirtualPath);
                    writer.writeEndElement();
                } else {
                    writer.writeStartElement(LVPROJ_KEY_PLACEHOLDER);
                    writer.writeAttribute(LVPROJ_ATTRIBUTE_ITEM_ID, item.Id);
                    writer.writeAttribute(LVPROJ_ATTRIBUTE_ITEM_SPEAKER, item.Speaker);
                    writer.writeAttribute(LVPROJ_ATTRIBUTE_ITEM_VIRTUAL_PATH, item.VirtualPath);
                    writer.writeEndElement();
                }
            }
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
    writer.writeEndDocument();
    file.close();

    return true;
}

void LVModel::ProjectModel::reset() {
    Version = "0.0.1";
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
            (reg && !registry.Register(language.Id))) {
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
            (reg && !registry.Register(speaker.Id))) {
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
        registry.Register(key);
    }
}
