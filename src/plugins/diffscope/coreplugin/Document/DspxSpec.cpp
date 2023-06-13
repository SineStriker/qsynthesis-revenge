#include "DspxSpec.h"
#include "DspxSpec_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include <QMDecoratorV2.h>

#include "DspxConst.h"
#include "ICore.h"
#include "Window/IProjectWindow.h"

namespace Core {

    static AceTreeSerializer *createDspxRootSerializer() {
        auto root = new AceTreeSerializer("root", QJsonValue::Object);

        // metadata
        {
            auto metadata = new AceTreeSerializer("root.metadata", QJsonValue::Object);
            metadata->setObjectAcceptOnes({
                {"version", {AceTreeSerializer::Property, Dspx::Version}           },
                {"author",  {AceTreeSerializer::Property, Dspx::DefaultAuthor}     },
                {"name",    {AceTreeSerializer::Property, Dspx::DefaultProjectName}},
            });
            root->addChild("metadata", metadata);
        }

        // workspace
        {
            auto workspace = new AceTreeSerializer("root.workspace", QJsonValue::Object);
            workspace->setObjectAcceptAll(AceTreeSerializer::DynamicData);
            root->addChild("workspace", workspace);
        }

        // content
        {
            auto content = new AceTreeSerializer("root.content", QJsonValue::Object);
            root->addChild("content", content);

            // workspace
            {
                auto workspace = new AceTreeSerializer("root.content.workspace", QJsonValue::Object);
                workspace->setObjectAcceptAll(AceTreeSerializer::DynamicData);
                content->addChild("workspace", workspace);
            }

            // master
            {
                auto master = new AceTreeSerializer("root.content.master", QJsonValue::Object);
                content->addChild("master", master);

                // control
                {
                    auto control = new AceTreeSerializer("root.content.master.control", QJsonValue::Object);
                    control->setObjectAcceptOnes({
                        {"gain", {AceTreeSerializer::Property, Dspx::DefaultGain}},
                        {"mute", {AceTreeSerializer::Property, Dspx::DefaultMute}},
                    });
                    master->addChild("control", control);
                }
            }

            // timeline
            {
                auto timeline = new AceTreeSerializer("root.content.timeline", QJsonValue::Object);
                content->addChild("timeline", timeline);

                // timeSignatures
                {
                    auto timeSignatures =
                        new AceTreeSerializer("root.content.timeline.timeSignatures", QJsonValue::Array);
                    timeline->addChild("timeSignatures", timeSignatures);

                    auto timeSignature =
                        new AceTreeSerializer("root.content.timeline.timeSignature", QJsonValue::Object);
                    timeSignature->setObjectAcceptOnes({
                        {"pos",         {AceTreeSerializer::Property, Dspx::DefaultTimeSignaturePos}        },
                        {"numerator",   {AceTreeSerializer::Property, Dspx::DefaultTimeSignatureNumerator}  },
                        {"denominator", {AceTreeSerializer::Property, Dspx::DefaultTimeSignatureDenominator}},
                    });
                    timeSignatures->addChild("item", timeSignature);
                }

                // tempos
                {
                    auto tempos = new AceTreeSerializer("root.content.timeline.tempos", QJsonValue::Array);
                    timeline->addChild("tempos", tempos);

                    auto tempo = new AceTreeSerializer("root.content.timeline.tempo", QJsonValue::Object);
                    tempo->setObjectAcceptOnes({
                        {"pos",   {AceTreeSerializer::Property, Dspx::DefaultTempoPos}  },
                        {"value", {AceTreeSerializer::Property, Dspx::DefaultTempoValue}},
                    });
                    tempos->addChild("item", tempo);
                }

                // labels
                {
                    auto labels = new AceTreeSerializer("root.content.timeline.labels", QJsonValue::Array);
                    timeline->addChild("labels", labels);

                    auto label = new AceTreeSerializer("root.content.timeline.label", QJsonValue::Object);
                    label->setObjectAcceptOnes({
                        {"pos",  {AceTreeSerializer::Property, Dspx::DefaultLabelPos} },
                        {"text", {AceTreeSerializer::Property, Dspx::DefaultLabelText}},
                    });
                    labels->addChild("item", label);
                }
            }

            // tracks
            {
                auto tracks = new AceTreeSerializer("root.content.tracks", QJsonValue::Array);
                content->addChild("tracks", tracks);

                auto track = new AceTreeSerializer("root.content.track", QJsonValue::Object);
                track->setObjectAcceptOnes({
                    {"name", {AceTreeSerializer::Property, Dspx::DefaultTrackName}},
                });
                tracks->addChild("item", track);

                // control
                {
                    auto control = new AceTreeSerializer("root.content.track.control", QJsonValue::Object);
                    control->setObjectAcceptOnes({
                        {"gain", {AceTreeSerializer::Property, Dspx::DefaultGain}},
                        {"pan",  {AceTreeSerializer::Property, Dspx::DefaultPan} },
                        {"mute", {AceTreeSerializer::Property, Dspx::DefaultMute}},
                        {"solo", {AceTreeSerializer::Property, Dspx::DefaultSolo}},
                    });
                    track->addChild("control", control);
                }

                // workspace
                {
                    auto workspace = new AceTreeSerializer("root.content.track.workspace", QJsonValue::Object);
                    workspace->setObjectAcceptAll(AceTreeSerializer::DynamicData);
                    track->addChild("workspace", workspace);
                }

                // clip
                {
                    auto clips = new AceTreeSerializer("root.content.track.clips", QJsonValue::Array);
                    clips->setArrayTypeField("type");
                    track->addChild("clips", clips);

                    // singing
                    {
                        auto singingClip =
                            new AceTreeSerializer("root.content.track.singingClip", QJsonValue::Object);
                        singingClip->setObjectAcceptOnes({
                            {"name", {AceTreeSerializer::Property, Dspx::DefaultSingingClipName}},
                        });
                        clips->addChild("singing", singingClip);

                        // time
                        {
                            auto time =
                                new AceTreeSerializer("root.content.track.singingClip.time", QJsonValue::Object);
                            time->setObjectAcceptOnes({
                                {"start",     {AceTreeSerializer::Property, Dspx::DefaultClipStart}     },
                                {"length",    {AceTreeSerializer::Property, Dspx::DefaultClipLength}    },
                                {"clipStart", {AceTreeSerializer::Property, Dspx::DefaultClipClipStart} },
                                {"clipLen",   {AceTreeSerializer::Property, Dspx::DefaultClipClipLength}},
                            });
                            singingClip->addChild("time", time);
                        }

                        // control
                        {
                            auto control = new AceTreeSerializer("root.content.track.singingClip.control",
                                                                        QJsonValue::Object);
                            control->setObjectAcceptOnes({
                                {"gain", {AceTreeSerializer::Property, Dspx::DefaultGain}},
                                {"mute", {AceTreeSerializer::Property, Dspx::DefaultMute}},
                            });
                            singingClip->addChild("control", control);
                        }

                        // workspace
                        {
                            auto workspace = new AceTreeSerializer("root.content.track.singingClip.workspace",
                                                                          QJsonValue::Object);
                            workspace->setObjectAcceptAll(AceTreeSerializer::DynamicData);
                            singingClip->addChild("workspace", workspace);
                        }

                        // notes
                        {
                            auto notes =
                                new AceTreeSerializer("root.content.track.singingClip.notes", QJsonValue::Array);
                            notes->setArrayUnordered(true);
                            singingClip->addChild("notes", notes);

                            auto note =
                                new AceTreeSerializer("root.content.track.singingClip.note", QJsonValue::Object);
                            note->setObjectAcceptOnes({
                                {"pos",    {AceTreeSerializer::Property, Dspx::DefaultNotePos}   },
                                {"length", {AceTreeSerializer::Property, Dspx::DefaultNoteLength}},
                                {"keyNum", {AceTreeSerializer::Property, Dspx::DefaultNoteKeyNum}},
                                {"lyric",  {AceTreeSerializer::Property, Dspx::DefaultNoteLyric} },
                            });
                            notes->addChild("item", note);

                            // workspace
                            {
                                auto workspace = new AceTreeSerializer(
                                    "root.content.track.singingClip.note.workspace", QJsonValue::Object);
                                workspace->setObjectAcceptAll(AceTreeSerializer::DynamicData);
                                note->addChild("workspace", workspace);
                            }

                            // vibrato
                            {
                                auto vibrato = new AceTreeSerializer(
                                    "root.content.track.singingClip.note.vibrato", QJsonValue::Object);
                                vibrato->setObjectAcceptOnes({
                                    {"start",  {AceTreeSerializer::Property, Dspx::DefaultVibratoStart}    },
                                    {"end",    {AceTreeSerializer::Property, Dspx::DefaultVibratoEnd}      },
                                    {"freq",   {AceTreeSerializer::Property, Dspx::DefaultVibratoFrequency}},
                                    {"phase",  {AceTreeSerializer::Property, Dspx::DefaultVibratoPhase}    },
                                    {"amp",    {AceTreeSerializer::Property, Dspx::DefaultVibratoAmplitude}},
                                    {"offset", {AceTreeSerializer::Property, Dspx::DefaultVibratoOffset}   },
                                });
                                note->addChild("vibrato", vibrato);

                                // points
                                {
                                    auto points = new AceTreeSerializer(
                                        "root.content.track.singingClip.note.vibrato.points", QJsonValue::Array);
                                    vibrato->addChild("points", points);

                                    auto point = new AceTreeSerializer(
                                        "root.content.track.singingClip.note.vibrato.point", QJsonValue::Object);
                                    point->setObjectAcceptOnes({
                                        {"x", {AceTreeSerializer::Property, Dspx::DefaultVibratoPointX}},
                                        {"y", {AceTreeSerializer::Property, Dspx::DefaultVibratoPointY}},
                                    });
                                    points->addChild("point", point);
                                }
                            }

                            // phonemes
                            {
                                auto phonemes = new AceTreeSerializer(
                                    "root.content.track.singingClip.note.phonemes", QJsonValue::Object);
                                phonemes->setObjectAcceptOnes({
                                    {"original", {AceTreeSerializer::DynamicData, QJsonValue::Array}},
                                });
                                note->addChild("phonemes", phonemes);

                                // edited
                                auto edited = new AceTreeSerializer(
                                    "root.content.track.singingClip.note.phonemes.edited", QJsonValue::Array);
                                phonemes->addChild("edited", edited);

                                // phoneme
                                auto phoneme = new AceTreeSerializer(
                                    "root.content.track.singingClip.note.phonemes.edited.phoneme", QJsonValue::Object);
                                phoneme->setObjectAcceptOnes({
                                    {"type",  {AceTreeSerializer::Property, Dspx::DefaultPhonemeType} },
                                    {"token", {AceTreeSerializer::Property, Dspx::DefaultPhonemeToken}},
                                    {"start", {AceTreeSerializer::Property, Dspx::DefaultPhonemeStart}},
                                });
                                edited->addChild("item", phoneme);
                            }
                        }

                        // params
                        {
                            auto params = new AceTreeSerializer("root.content.track.singingClip.note.params",
                                                                       QJsonValue::Object);
                            singingClip->addChild("params", params);

                            // pitch
                            {
                                auto pitch = new AceTreeSerializer(
                                    "root.content.track.singingClip.note.params.pitch", QJsonValue::Object);
                                pitch->setObjectAcceptOnes({
                                    {"original", {AceTreeSerializer::DynamicData, QJsonValue::Array}},
                                });
                                params->addChild("pitch", pitch);

                                // edited
                                auto edited = new AceTreeSerializer(
                                    "root.content.track.singingClip.note.params.pitch.edited", QJsonValue::Array);
                                edited->setArrayTypeField("type");
                                pitch->addChild("edited", edited);

                                // anchor
                                {
                                    auto anchor = new AceTreeSerializer(
                                        "root.content.track.singingClip.note.params.pitch.edited.anchor",
                                        QJsonValue::Object);
                                    edited->addChild("anchor", anchor);

                                    // nodes
                                    auto nodes = new AceTreeSerializer(
                                        "root.content.track.singingClip.note.params.pitch.edited.anchor.nodes",
                                        QJsonValue::Array);
                                    nodes->setArrayUnordered(true);
                                    anchor->addChild("nodes", nodes);

                                    // node
                                    auto node = new AceTreeSerializer(
                                        "root.content.track.singingClip.note.params.pitch.edited.anchor.node",
                                        QJsonValue::Object);
                                    node->setObjectAcceptOnes({
                                        {"x",      {AceTreeSerializer::Property, Dspx::DefaultParamAnchorNodeX}},
                                        {"y",      {AceTreeSerializer::Property, Dspx::DefaultParamAnchorNodeY}},
                                        {"interp",
                                         {AceTreeSerializer::Property,
                                          Dspx::DefaultParamAnchorNodeInterpolation}                                  },
                                    });
                                    nodes->addChild("item", node);
                                }

                                // free
                                {
                                    auto free = new AceTreeSerializer(
                                        "root.content.track.singingClip.note.params.pitch.edited.free",
                                        QJsonValue::Object);
                                    free->setObjectOptions({
                                        {},
                                        {
                                         {"start",
                                             {AceTreeSerializer::Property, Dspx::DefaultParamFreeStart}},
                                         {"step", {AceTreeSerializer::Property, Dspx::DefaultParamFreeStep}},
                                         },

                                        [](const QString &key, const QJsonValue &value, AceTreeItem *item,
                                           void *) {
                                            if (key != "values")
                                                return AceTreeSerializer::Unhandled;
                                            if (!value.isArray())
                                                return AceTreeSerializer::Failed;

                                            QByteArray bytes;
                                            QDataStream stream(&bytes, QIODevice::WriteOnly);
                                            for (const auto &child : value.toArray()) {
                                                stream << qint16(child.isDouble() ? child.toDouble() : 0);
                                            }

                                            auto childItem = new AceTreeItem("values");
                                            childItem->setBytes(0, bytes);
                                            item->addNode(childItem);

                                            return AceTreeSerializer::Success;
                                         },

                                        [](const QString &key, QJsonObject *obj,
                                           const AceTreeSerializer::ChildOrProperty &cop, void *) {
                                            if (key != "values")
                                                return AceTreeSerializer::Unhandled;
                                            if (!cop.is_item) {
                                                return AceTreeSerializer::Failed;
                                            }

                                            QByteArray bytes = cop.i->bytes();
                                            QDataStream stream(&bytes, QIODevice::ReadOnly);

                                            QJsonArray arr;
                                            while (stream.device()->pos() < bytes.size()) {
                                                qint16 num;
                                                stream >> num;
                                                if (stream.status() != QDataStream::Ok) {
                                                    break;
                                                }
                                                arr.append(double(num));
                                            }
                                            obj->insert("values", arr);

                                            return AceTreeSerializer::Success;
                                         },

                                        [](AceTreeItem *item, void *) {
                                            auto childItem = new AceTreeItem("values");
                                            item->addNode(childItem);
                                            return AceTreeSerializer::Success;
                                         },

                                        nullptr,
                                    });
                                    edited->addChild("free", free);
                                }
                            }
                        }
                    }

                    // audio
                    {
                        auto audioClip =
                            new AceTreeSerializer("root.content.track.audioClip", QJsonValue::Object);
                        audioClip->setObjectAcceptOnes({
                            {"name", {AceTreeSerializer::Property, Dspx::DefaultAudioClipName}},
                            {"path", {AceTreeSerializer::Property, Dspx::DefaultAudioClipPath}},
                        });
                        clips->addChild("audio", audioClip);

                        // time
                        {
                            auto time =
                                new AceTreeSerializer("root.content.track.audioClip.time", QJsonValue::Object);
                            time->setObjectAcceptOnes({
                                {"start",     {AceTreeSerializer::Property, Dspx::DefaultClipStart}     },
                                {"length",    {AceTreeSerializer::Property, Dspx::DefaultClipLength}    },
                                {"clipStart", {AceTreeSerializer::Property, Dspx::DefaultClipClipStart} },
                                {"clipLen",   {AceTreeSerializer::Property, Dspx::DefaultClipClipLength}},
                            });
                            audioClip->addChild("time", time);
                        }

                        // control
                        {
                            auto control = new AceTreeSerializer("root.content.track.audioClip.control",
                                                                        QJsonValue::Object);
                            control->setObjectAcceptOnes({
                                {"gain", {AceTreeSerializer::Property, Dspx::DefaultGain}},
                                {"mute", {AceTreeSerializer::Property, Dspx::DefaultMute}},
                            });
                            audioClip->addChild("control", control);
                        }

                        // workspace
                        {
                            auto workspace = new AceTreeSerializer("root.content.track.audioClip.workspace",
                                                                          QJsonValue::Object);
                            workspace->setObjectAcceptAll(AceTreeSerializer::DynamicData);
                            audioClip->addChild("workspace", workspace);
                        }
                    }
                }
            }
        }
        return root;
    }

    DspxSpecPrivate::DspxSpecPrivate(Core::DspxSpec *q) : q(q) {
        serializer = createDspxRootSerializer();
    }

    DspxSpecPrivate::~DspxSpecPrivate() {
    }

    void DspxSpecPrivate::init() {
    }

    DspxSpec *m_instance = nullptr;

    DspxSpec::DspxSpec(QObject *parent) : DocumentSpec("org.ChorusKit.dspx", parent), d(new DspxSpecPrivate(this)) {
        m_instance = this;

        d->init();

        setIcon(QIcon(":/images/dspx.png"));
        setDisplayName([]() { return tr("OpenVPI DiffSinger Editor"); });
        setDescription([]() { return tr("Built-in"); });
    }

    DspxSpec::~DspxSpec() {
        delete d;

        m_instance = nullptr;
    }

    DspxSpec *DspxSpec::instance() {
        return m_instance;
    }

    QStringList DspxSpec::supportedExtensions() const {
        return {"dspx"};
    }

    QString DspxSpec::filter() const {
        return QString("%1(%2)").arg(tr("DiffScope Project Files"), "*.dspx");
    }

    bool DspxSpec::open(const QString &fileName) {
        if (DocumentSpec::open(fileName))
            return true;

        auto iWin = ICore::instance()->windowSystem()->createWindow("project")->cast<IProjectWindow>();
        if (!iWin)
            return false;

        auto doc = iWin->doc();
        if (!doc->open(fileName)) {
            QMessageBox::critical(nullptr, tr("File Error"), doc->errorMessage());
            iWin->window()->close();
            return false;
        }

        return true;
    }

    bool DspxSpec::canRecover() const {
        return true;
    }

    bool DspxSpec::recover(const QString &logDir, const QString &fileName) {
        auto iWin = ICore::instance()->windowSystem()->createWindow("project")->cast<IProjectWindow>();
        if (!iWin)
            return false;

        auto doc = iWin->doc();
        doc->setLogDirectory(logDir);

        if (!doc->open(fileName)) {
            QMessageBox::critical(nullptr, tr("File Error"), doc->errorMessage());

            if (qApp->property("closeHomeOnOpen").toBool())
                QTimer::singleShot(0, iWin->window(), &QWidget::close);

            return false;
        }

        return true;
    }

    AceTreeSerializer *DspxSpec::serializer() const {
        return d->serializer;
    }

} // Core