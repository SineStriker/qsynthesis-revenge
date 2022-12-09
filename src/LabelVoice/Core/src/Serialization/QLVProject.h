#ifndef QLVPROJECT_H
#define QLVPROJECT_H

#include <QMap>
#include <QStringList>

#include "Utils/HexGenerator.h"

#include "lvcore_global.h"

namespace LVModel {

    enum class LayerCategory { Sentence, Grapheme, Phoneme, Pitch, Duration, Custom };

    enum class ValueType { Text, Integer, Float, Pitch };

    LVCORE_API QString LayerCategoryToString(LayerCategory c);
    LVCORE_API LayerCategory StringToLayerCategory(const QString &s);

    LVCORE_API QString ValueTypeToString(ValueType c);
    LVCORE_API ValueType StringToValueType(const QString &s);

    constexpr int HexIdLength = 4;

    class LVCORE_API LayerDefinition {
    public:
        LayerCategory Category;
        QString Name;
        int SubdivisionOf;
        int AlignedWith;
        ValueType ValType;

        LayerDefinition();
        ~LayerDefinition();
    };

    class LVCORE_API LanguageDefinition {
    public:
        QString Id;
        QString Name;
        QString DictionaryPath;
        QString PhonemeSetPath;
        QString AlignerRootPath;
    };

    class LVCORE_API SpeakerDefinition {
    public:
        QString Id;
        QString Name;
    };

    class LVCORE_API ItemResource {
    public:
        enum ResourceType { Definition, Placeholder };

        ResourceType Type;

        QString Id;
        QString Name;
        QString Speaker;
        QString Language;
        QString VirtualPath;

        ItemResource(ResourceType type = Placeholder);
        ~ItemResource();

        bool operator<(const ItemResource &other) const;
    };


    class LVCORE_API ProjectModel {
    public:
        ProjectModel();
        ~ProjectModel();

    public:
        bool load(const QString &filename);
        bool save(const QString &filename);

        void reset();

        void Validate(bool reg = false);
        void ValidateLayers();
        void ValidateLanguages(bool reg = false);
        void ValidateSpeakers(bool reg = false);
        void ValidateItemResources(bool reg = false);

    public:
        QString Version;
        QString Name;
        QList<LayerDefinition> LabelSchema;
        QList<LanguageDefinition> Languages;
        QList<SpeakerDefinition> Speakers;
        QMap<QString, ItemResource> ItemResources;

        HexGenerator hexIdPublisher;
    };

} // namespace LVModel

#endif // QLVPROJECT_H
