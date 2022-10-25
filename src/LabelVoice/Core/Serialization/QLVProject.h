#ifndef QLVPROJECT_H
#define QLVPROJECT_H

#include <QMap>
#include <QStringList>

namespace LVModel {

    enum class LayerCategory { Sentence, Grapheme, Phoneme, Pitch, Duration, Custom };

    enum class ValueType { Text, Integer, Float, Pitch };

    class LayerDefinition {
    public:
        LayerCategory Category;
        QString Name;
        int SubdivisionOf;
        int AlignedWith;
        ValueType ValType;

        LayerDefinition();
    };

    class LanguageDefinition {
    public:
        QString Id;
        QString Name;
        QString DictionaryPath;
        QString PhonemeSetPath;
        QString AlignerRootPath;
    };

    class SpeakerDefinition {
    public:
        QString Id;
        QString Name;
    };

    class ItemResource {
    public:
        enum ResourceType { Definition, Placeholder };

        ResourceType Type;

        QString Id;
        QString Speaker;
        QString VirtualPath;

        QString Name;
        QString Language;

        ItemResource(ResourceType type = Placeholder);
        bool operator<(const ItemResource &other) const;
    };


    class ProjectModel {
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
    };

} // namespace LVProj

#endif // QLVPROJECT_H
