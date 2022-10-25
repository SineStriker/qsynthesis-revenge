#include "QLVItem.h"

#include <QDebug>
#include <QFile>
#include <QXmlStreamReader>

static const char LVITEM_ROOT[] = "LVItem";
static const char LVITEM_KEY_VERSION[] = "Version";
static const char LVITEM_KEY_ID[] = "Id";
static const char LVITEM_KEY_NAME[] = "Name";
static const char LVITEM_KEY_OWNER_PROJECT[] = "OwnerProject";
static const char LVITEM_KEY_AUDIO_SOURCE[] = "AudioSource";

static const char LVITEM_SECTION_SLICES[] = "Slices";
static const char LVITEM_KEY_SLICE[] = "Slice";
static const char LVITEM_ATTRIBUTE_SLICE_ID[] = "Id";
static const char LVITEM_ATTRIBUTE_SLICE_IN[] = "In";
static const char LVITEM_ATTRIBUTE_SLICE_OUT[] = "Out";
static const char LVITEM_ATTRIBUTE_SLICE_LANGUAGE[] = "Language";

LVModel::SliceDefinition::SliceDefinition() {
    In = Out = 0;
}

LVModel::SliceDefinition::~SliceDefinition() {
}

bool LVModel::SliceDefinition::operator<(const LVModel::SliceDefinition &other) const {
    if (In < other.In) {
        return true;
    }
    return Out < other.Out;
}


LVModel::ItemModel::ItemModel(HexGenerator &hexGen) : registry(hexGen) {
    reset();
}

LVModel::ItemModel::~ItemModel() {
}

bool LVModel::ItemModel::load(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QXmlStreamReader reader(&file);
    bool success = true;
    enum class Sections {
        Single,
        Outer,
        LVItem,
        Slices,
    };

    std::list<Sections> sections;
    sections.push_back(Sections::Outer);

    while (!reader.atEnd()) {
        QXmlStreamReader::TokenType nType = reader.readNext();

        switch (nType) {
            case QXmlStreamReader::StartDocument: {
                qDebug() << "********** Start XML ********** ";
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
                        if (name == LVITEM_ROOT) {
                            // Enter LVItem
                            sections.push_back(Sections::LVItem);
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::LVItem:
                        if (name == LVITEM_KEY_VERSION) {
                            // Parse LVItem.Version
                            reader.readNext();
                            if (reader.isCharacters()) {
                                Version = reader.text().toString();
                            } else {
                                success = false;
                            }
                            sections.push_back(Sections::Single);
                        } else if (name == LVITEM_KEY_ID) {
                            // Parse LVItem.Id
                            reader.readNext();
                            if (reader.isCharacters()) {
                                Id = reader.text().toString();
                            } else {
                                success = false;
                            }
                            sections.push_back(Sections::Single);
                        } else if (name == LVITEM_KEY_NAME) {
                            // Parse LVItem.Name
                            reader.readNext();
                            if (reader.isCharacters()) {
                                Name = reader.text().toString();
                            } else {
                                success = false;
                            }
                            sections.push_back(Sections::Single);
                        } else if (name == LVITEM_KEY_OWNER_PROJECT) {
                            // Parse LVItem.OwnerProject
                            reader.readNext();
                            if (reader.isCharacters()) {
                                OwnerProject = reader.text().toString();
                            } else {
                                success = false;
                            }
                            sections.push_back(Sections::Single);
                        } else if (name == LVITEM_KEY_AUDIO_SOURCE) {
                            // Parse LVItem.AudioSource
                            reader.readNext();
                            if (reader.isCharacters()) {
                                AudioSource = reader.text().toString();
                            } else {
                                success = false;
                            }
                            sections.push_back(Sections::Single);
                        } else if (name == LVITEM_SECTION_SLICES) {
                            // Enter LVItem.Slices
                            sections.push_back(Sections::Slices);
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::Slices:
                        if (name == LVITEM_KEY_SLICE) {
                            // Parse LVItem.Slices.Slice
                            SliceDefinition slice;
                            QXmlStreamAttributes attrs = reader.attributes();
                            for (const auto &attr : qAsConst(attrs)) {
                                QString name = attr.name().toString();
                                QString val = attr.value().toString();
                                if (name == LVITEM_ATTRIBUTE_SLICE_ID) {
                                    slice.Id = val;
                                } else if (name == LVITEM_ATTRIBUTE_SLICE_IN) {
                                    slice.In = val.toDouble();
                                } else if (name == LVITEM_ATTRIBUTE_SLICE_OUT) {
                                    slice.Out = val.toDouble();
                                } else if (name == LVITEM_ATTRIBUTE_SLICE_LANGUAGE) {
                                    slice.Language = val;
                                }
                            }
                            Slices.insert(slice.Id, slice);
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
                    case Sections::LVItem:
                        if (name == LVITEM_ROOT) {
                            // Exit LVItem
                            sections.pop_back();
                        } else {
                            success = false;
                        }
                        break;
                    case Sections::Slices:
                        if (name == LVITEM_SECTION_SLICES) {
                            // Exit LVItem.Slices
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
                qDebug() << QString::fromLocal8Bit("********** End XML ********** ");
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
        reader.readNext();
    }

    file.close();
    return success;
}

bool LVModel::ItemModel::save(const QString &filename) {
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        return false;
    }

    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.writeStartDocument();
    {
        // LVItem
        writer.writeStartElement(LVITEM_ROOT);
        {
            // LVItem.Version
            writer.writeStartElement(LVITEM_KEY_VERSION);
            writer.writeCharacters(Version);
            writer.writeEndElement();

            // LVItem.Id
            writer.writeStartElement(LVITEM_KEY_ID);
            writer.writeCharacters(Id);
            writer.writeEndElement();

            // LVItem.Name
            writer.writeStartElement(LVITEM_KEY_NAME);
            writer.writeCharacters(Name);
            writer.writeEndElement();

            // LVItem.OwnerProject
            writer.writeStartElement(LVITEM_KEY_OWNER_PROJECT);
            writer.writeCharacters(OwnerProject);
            writer.writeEndElement();

            // LVItem.AudioSource
            writer.writeStartElement(LVITEM_KEY_AUDIO_SOURCE);
            writer.writeCharacters(AudioSource);
            writer.writeEndElement();

            // LVItem.LabelSchema
            writer.writeStartElement(LVITEM_SECTION_SLICES);
            for (const SliceDefinition &slice : qAsConst(Slices)) {
                writer.writeStartElement(LVITEM_KEY_SLICE);
                writer.writeAttribute(LVITEM_ATTRIBUTE_SLICE_ID, slice.Id);
                writer.writeAttribute(LVITEM_ATTRIBUTE_SLICE_IN, QString::number(slice.In));
                writer.writeAttribute(LVITEM_ATTRIBUTE_SLICE_OUT, QString::number(slice.Out));
                writer.writeAttribute(LVITEM_ATTRIBUTE_SLICE_LANGUAGE, slice.Language);
                writer.writeEndElement();
            }
            writer.writeEndElement();
        }
        writer.writeEndElement();
    }
    writer.writeEndDocument();
    file.close();
    return true;
}

void LVModel::ItemModel::reset() {
    Version.clear();
    Id.clear();
    Name.clear();
    OwnerProject.clear();
    AudioSource.clear();
    Slices.clear();
}

void LVModel::ItemModel::Validate(bool reg) {
    for (auto it = Slices.begin(); it != Slices.end(); ++it) {
        QString key = it.key();
        const auto &val = it.value();
        if (!HexGenerator::IsValidFormat(key, 8) || val.Out < val.In) {
            it = Slices.erase(it);
        }
    }

    /*
            About overlapping detection:
            Overlapping is useful in some cases like sharing the same silence parts between two
       slices. For this reason, there is no overlapping detection here. It is up to the UI and
       ViewModel implementations.
            */

    if (!reg) {
        return;
    }

    QStringList keys = Slices.keys();
    foreach (const auto &key, qAsConst(keys)) {
        registry.Register(key);
    }
}
