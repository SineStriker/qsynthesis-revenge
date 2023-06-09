#include "DspxSpec.h"
#include "DspxSpec_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include <QMDecoratorV2.h>

#include "DspxConst.h"
#include "DspxController.h"
#include "ICore.h"
#include "Window/IProjectWindow.h"

namespace Core {

    static QsApi::AceTreeSerializer *createDspxRootSerializer() {
        auto root = new QsApi::AceTreeSerializer("root", QJsonValue::Object);

        // metadata
        {
            auto metadata = new QsApi::AceTreeSerializer("root.metadata", QJsonValue::Object);
            metadata->setObjectAcceptOnes({
                {"version", {QsApi::AceTreeSerializer::Property, Dspx::Version}           },
                {"author",  {QsApi::AceTreeSerializer::Property, Dspx::DefaultAuthor}     },
                {"name",    {QsApi::AceTreeSerializer::Property, Dspx::DefaultProjectName}},
            });
            root->addChild("metadata", metadata);
        }

        // workspace
        {
            auto workspace = new QsApi::AceTreeSerializer("root.workspace", QJsonValue::Object);
            workspace->setObjectAcceptAll(QsApi::AceTreeSerializer::DynamicData);
            root->addChild("workspace", workspace);
        }

        // content
        {
            auto content = new QsApi::AceTreeSerializer("root.content", QJsonValue::Object);
            root->addChild("content", content);

            // workspace
            {
                auto workspace = new QsApi::AceTreeSerializer("root.content.workspace", QJsonValue::Object);
                workspace->setObjectAcceptAll(QsApi::AceTreeSerializer::DynamicData);
                content->addChild("workspace", workspace);
            }

            // master
            {
                auto master = new QsApi::AceTreeSerializer("root.content.master", QJsonValue::Object);
                content->addChild("master", master);

                // control
                {
                    auto control = new QsApi::AceTreeSerializer("root.content.master.control", QJsonValue::Object);
                    control->setObjectAcceptOnes({
                        {"gain", {QsApi::AceTreeSerializer::Property, Dspx::DefaultGain}},
                        {"mute", {QsApi::AceTreeSerializer::Property, Dspx::DefaultMute}},
                    });
                    master->addChild("control", control);
                }
            }

            // timeline
            {
                auto timeline = new QsApi::AceTreeSerializer("root.content.timeline", QJsonValue::Object);
                content->addChild("timeline", timeline);

                // timeSignatures
                {
                    auto timeSignatures =
                        new QsApi::AceTreeSerializer("root.content.timeline.timeSignatures", QJsonValue::Array);
                    timeline->addChild("timeSignatures", timeSignatures);

                    auto timeSignature =
                        new QsApi::AceTreeSerializer("root.content.timeline.timeSignature", QJsonValue::Object);
                    timeSignature->setObjectAcceptOnes({
                        {"pos",         {QsApi::AceTreeSerializer::Property, Dspx::DefaultTimeSignaturePos}        },
                        {"numerator",   {QsApi::AceTreeSerializer::Property, Dspx::DefaultTimeSignatureNumerator}  },
                        {"denominator", {QsApi::AceTreeSerializer::Property, Dspx::DefaultTimeSignatureDenominator}},
                    });
                    timeSignatures->addChild("item", timeSignature);
                }

                // tempos
                {
                    auto tempos = new QsApi::AceTreeSerializer("root.content.timeline.tempos", QJsonValue::Array);
                    timeline->addChild("tempos", tempos);

                    auto tempo = new QsApi::AceTreeSerializer("root.content.timeline.tempo", QJsonValue::Object);
                    tempo->setObjectAcceptOnes({
                        {"pos",   {QsApi::AceTreeSerializer::Property, Dspx::DefaultTempoPos}  },
                        {"value", {QsApi::AceTreeSerializer::Property, Dspx::DefaultTempoValue}},
                    });
                    tempos->addChild("item", tempo);
                }

                // labels
                {
                    auto labels = new QsApi::AceTreeSerializer("root.content.timeline.labels", QJsonValue::Array);
                    timeline->addChild("labels", labels);

                    auto label = new QsApi::AceTreeSerializer("root.content.timeline.label", QJsonValue::Object);
                    label->setObjectAcceptOnes({
                        {"pos",  {QsApi::AceTreeSerializer::Property, Dspx::DefaultLabelPos} },
                        {"text", {QsApi::AceTreeSerializer::Property, Dspx::DefaultLabelText}},
                    });
                    labels->addChild("item", label);
                }
            }

            // tracks
            {
                auto tracks = new QsApi::AceTreeSerializer("root.content.tracks", QJsonValue::Array);
                content->addChild("tracks", tracks);

                auto track = new QsApi::AceTreeSerializer("root.content.track", QJsonValue::Object);
                track->setObjectAcceptOnes({
                    {"name", {QsApi::AceTreeSerializer::Property, Dspx::DefaultTrackName}},
                });
                tracks->addChild("item", track);

                // control
                {
                    auto control = new QsApi::AceTreeSerializer("root.content.track.control", QJsonValue::Object);
                    control->setObjectAcceptOnes({
                        {"gain", {QsApi::AceTreeSerializer::Property, Dspx::DefaultGain}},
                        {"pan",  {QsApi::AceTreeSerializer::Property, Dspx::DefaultPan} },
                        {"mute", {QsApi::AceTreeSerializer::Property, Dspx::DefaultMute}},
                        {"solo", {QsApi::AceTreeSerializer::Property, Dspx::DefaultSolo}},
                    });
                    track->addChild("control", control);
                }

                // workspace
                {
                    auto workspace = new QsApi::AceTreeSerializer("root.content.track.workspace", QJsonValue::Object);
                    workspace->setObjectAcceptAll(QsApi::AceTreeSerializer::DynamicData);
                    track->addChild("workspace", workspace);
                }

                // clip
                {
                    auto clips = new QsApi::AceTreeSerializer("root.content.track.clips", QJsonValue::Array);
                    clips->setArrayTypeField("type");
                    track->addChild("clips", clips);

                    // singing
                    {
                        auto singingClip =
                            new QsApi::AceTreeSerializer("root.content.track.singingClip", QJsonValue::Object);
                        singingClip->setObjectAcceptOnes({
                            {"name", {QsApi::AceTreeSerializer::Property, Dspx::DefaultSingingClipName}},
                        });
                        clips->addChild("singing", singingClip);

                        // time
                        {
                            auto time =
                                new QsApi::AceTreeSerializer("root.content.track.singingClip.time", QJsonValue::Object);
                            time->setObjectAcceptOnes({
                                {"start",     {QsApi::AceTreeSerializer::Property, Dspx::DefaultClipStart}     },
                                {"length",    {QsApi::AceTreeSerializer::Property, Dspx::DefaultClipLength}    },
                                {"clipStart", {QsApi::AceTreeSerializer::Property, Dspx::DefaultClipClipStart} },
                                {"clipLen",   {QsApi::AceTreeSerializer::Property, Dspx::DefaultClipClipLength}},
                            });
                            singingClip->addChild("time", time);
                        }

                        // control
                        {
                            auto control = new QsApi::AceTreeSerializer("root.content.track.singingClip.control",
                                                                        QJsonValue::Object);
                            control->setObjectAcceptOnes({
                                {"gain", {QsApi::AceTreeSerializer::Property, Dspx::DefaultGain}},
                                {"mute", {QsApi::AceTreeSerializer::Property, Dspx::DefaultMute}},
                            });
                            singingClip->addChild("control", control);
                        }

                        // workspace
                        {
                            auto workspace = new QsApi::AceTreeSerializer("root.content.track.singingClip.workspace",
                                                                          QJsonValue::Object);
                            workspace->setObjectAcceptAll(QsApi::AceTreeSerializer::DynamicData);
                            singingClip->addChild("workspace", workspace);
                        }

                        // notes
                        {
                            auto notes =
                                new QsApi::AceTreeSerializer("root.content.track.singingClip.notes", QJsonValue::Array);
                            singingClip->addChild("notes", notes);

                            auto note =
                                new QsApi::AceTreeSerializer("root.content.track.singingClip.note", QJsonValue::Object);
                            note->setObjectAcceptOnes({
                                {"pos",    {QsApi::AceTreeSerializer::Property, Dspx::DefaultNotePos}   },
                                {"length", {QsApi::AceTreeSerializer::Property, Dspx::DefaultNoteLength}},
                                {"keyNum", {QsApi::AceTreeSerializer::Property, Dspx::DefaultNoteKeyNum}},
                                {"lyric",  {QsApi::AceTreeSerializer::Property, Dspx::DefaultNoteLyric} },
                            });
                            notes->addChild("item", note);

                            // workspace
                            {
                                auto workspace = new QsApi::AceTreeSerializer(
                                    "root.content.track.singingClip.note.workspace", QJsonValue::Object);
                                workspace->setObjectAcceptAll(QsApi::AceTreeSerializer::DynamicData);
                                note->addChild("workspace", workspace);
                            }

                            // vibrato
                            {
                                auto vibrato = new QsApi::AceTreeSerializer(
                                    "root.content.track.singingClip.note.vibrato", QJsonValue::Object);
                                vibrato->setObjectAcceptOnes({
                                    {"start",  {QsApi::AceTreeSerializer::Property, Dspx::DefaultVibratoStart}    },
                                    {"end",    {QsApi::AceTreeSerializer::Property, Dspx::DefaultVibratoEnd}      },
                                    {"freq",   {QsApi::AceTreeSerializer::Property, Dspx::DefaultVibratoFrequency}},
                                    {"phase",  {QsApi::AceTreeSerializer::Property, Dspx::DefaultVibratoPhase}    },
                                    {"amp",    {QsApi::AceTreeSerializer::Property, Dspx::DefaultVibratoAmplitude}},
                                    {"offset", {QsApi::AceTreeSerializer::Property, Dspx::DefaultVibratoOffset}   },
                                });
                                note->addChild("vibrato", vibrato);

                                // points
                                {
                                    auto points = new QsApi::AceTreeSerializer(
                                        "root.content.track.singingClip.note.vibrato.points", QJsonValue::Array);
                                    vibrato->addChild("points", points);

                                    auto point = new QsApi::AceTreeSerializer(
                                        "root.content.track.singingClip.note.vibrato.point", QJsonValue::Object);
                                    point->setObjectAcceptOnes({
                                        {"x", {QsApi::AceTreeSerializer::Property, Dspx::DefaultVibratoPointX}},
                                        {"y", {QsApi::AceTreeSerializer::Property, Dspx::DefaultVibratoPointY}},
                                    });
                                    points->addChild("point", point);
                                }
                            }

                            // phonemes
                            {
                                auto phonemes = new QsApi::AceTreeSerializer(
                                    "root.content.track.singingClip.note.phonemes", QJsonValue::Object);
                                phonemes->setObjectAcceptOnes({
                                    {"original", {QsApi::AceTreeSerializer::DynamicData, QJsonValue::Array}},
                                });
                                note->addChild("phonemes", phonemes);

                                // edited
                                auto edited = new QsApi::AceTreeSerializer(
                                    "root.content.track.singingClip.note.phonemes.edited", QJsonValue::Array);
                                phonemes->addChild("edited", edited);

                                // phoneme
                                auto phoneme = new QsApi::AceTreeSerializer(
                                    "root.content.track.singingClip.note.phonemes.edited.phoneme", QJsonValue::Object);
                                phoneme->setObjectAcceptOnes({
                                    {"type",  {QsApi::AceTreeSerializer::Property, Dspx::DefaultPhonemeType} },
                                    {"token", {QsApi::AceTreeSerializer::Property, Dspx::DefaultPhonemeToken}},
                                    {"start", {QsApi::AceTreeSerializer::Property, Dspx::DefaultPhonemeStart}},
                                });
                                edited->addChild("item", phoneme);
                            }
                        }

                        // params
                        {
                            auto params = new QsApi::AceTreeSerializer("root.content.track.singingClip.note.params",
                                                                       QJsonValue::Object);
                            singingClip->addChild("params", params);

                            // pitch
                            {
                                auto pitch = new QsApi::AceTreeSerializer(
                                    "root.content.track.singingClip.note.params.pitch", QJsonValue::Object);
                                pitch->setObjectAcceptOnes({
                                    {"original", {QsApi::AceTreeSerializer::DynamicData, QJsonValue::Array}},
                                });
                                params->addChild("pitch", pitch);

                                // edited
                                auto edited = new QsApi::AceTreeSerializer(
                                    "root.content.track.singingClip.note.params.pitch.edited", QJsonValue::Array);
                                edited->setArrayTypeField("type");
                                pitch->addChild("edited", edited);

                                // anchor
                                {
                                    auto anchor = new QsApi::AceTreeSerializer(
                                        "root.content.track.singingClip.note.params.pitch.edited.anchor",
                                        QJsonValue::Object);
                                    edited->addChild("anchor", anchor);

                                    // nodes
                                    auto nodes = new QsApi::AceTreeSerializer(
                                        "root.content.track.singingClip.note.params.pitch.edited.anchor.nodes",
                                        QJsonValue::Array);
                                    anchor->addChild("nodes", nodes);

                                    // node
                                    auto node = new QsApi::AceTreeSerializer(
                                        "root.content.track.singingClip.note.params.pitch.edited.anchor.node",
                                        QJsonValue::Object);
                                    node->setObjectAcceptOnes({
                                        {"x",      {QsApi::AceTreeSerializer::Property, Dspx::DefaultParamAnchorNodeX}},
                                        {"y",      {QsApi::AceTreeSerializer::Property, Dspx::DefaultParamAnchorNodeY}},
                                        {"interp",
                                         {QsApi::AceTreeSerializer::Property,
                                          Dspx::DefaultParamAnchorNodeInterpolation}                                  },
                                    });
                                    nodes->addChild("item", node);
                                }

                                // free
                                {
                                    auto free = new QsApi::AceTreeSerializer(
                                        "root.content.track.singingClip.note.params.pitch.edited.free",
                                        QJsonValue::Object);
                                    free->setObjectOptions({
                                        {},
                                        {
                                         {"start",
                                             {QsApi::AceTreeSerializer::Property, Dspx::DefaultParamFreeStart}},
                                         {"step", {QsApi::AceTreeSerializer::Property, Dspx::DefaultParamFreeStep}},
                                         },

                                        [](const QString &key, const QJsonValue &value, QsApi::AceTreeItem *item,
                                           void *) {
                                            if (key != "values")
                                                return QsApi::AceTreeSerializer::Unhandled;
                                            if (!value.isArray())
                                                return QsApi::AceTreeSerializer::Failed;

                                            QByteArray bytes;
                                            QDataStream stream(&bytes, QIODevice::WriteOnly);
                                            for (const auto &child : value.toArray()) {
                                                stream << qint16(child.isDouble() ? child.toDouble() : 0);
                                            }

                                            auto childItem = new QsApi::AceTreeItem("values");
                                            childItem->setBytes(0, bytes);
                                            item->addNode(childItem);

                                            return QsApi::AceTreeSerializer::Success;
                                         },

                                        [](const QString &key, QJsonObject *obj,
                                           const QsApi::AceTreeSerializer::ChildOrProperty &cop, void *) {
                                            if (key != "values")
                                                return QsApi::AceTreeSerializer::Unhandled;
                                            if (!cop.is_item) {
                                                return QsApi::AceTreeSerializer::Failed;
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

                                            return QsApi::AceTreeSerializer::Success;
                                         },

                                        [](QsApi::AceTreeItem *item, void *) {
                                            auto childItem = new QsApi::AceTreeItem("values");
                                            item->addNode(childItem);
                                         },
                                    });
                                    edited->addChild("free", free);
                                }
                            }
                        }
                    }

                    // audio
                    {
                        auto audioClip =
                            new QsApi::AceTreeSerializer("root.content.track.audioClip", QJsonValue::Object);
                        audioClip->setObjectAcceptOnes({
                            {"name", {QsApi::AceTreeSerializer::Property, Dspx::DefaultAudioClipName}},
                            {"path", {QsApi::AceTreeSerializer::Property, Dspx::DefaultAudioClipPath}},
                        });
                        clips->addChild("audio", audioClip);

                        // time
                        {
                            auto time =
                                new QsApi::AceTreeSerializer("root.content.track.audioClip.time", QJsonValue::Object);
                            time->setObjectAcceptOnes({
                                {"start",     {QsApi::AceTreeSerializer::Property, Dspx::DefaultClipStart}     },
                                {"length",    {QsApi::AceTreeSerializer::Property, Dspx::DefaultClipLength}    },
                                {"clipStart", {QsApi::AceTreeSerializer::Property, Dspx::DefaultClipClipStart} },
                                {"clipLen",   {QsApi::AceTreeSerializer::Property, Dspx::DefaultClipClipLength}},
                            });
                            audioClip->addChild("time", time);
                        }

                        // control
                        {
                            auto control = new QsApi::AceTreeSerializer("root.content.track.audioClip.control",
                                                                        QJsonValue::Object);
                            control->setObjectAcceptOnes({
                                {"gain", {QsApi::AceTreeSerializer::Property, Dspx::DefaultGain}},
                                {"mute", {QsApi::AceTreeSerializer::Property, Dspx::DefaultMute}},
                            });
                            audioClip->addChild("control", control);
                        }

                        // workspace
                        {
                            auto workspace = new QsApi::AceTreeSerializer("root.content.track.audioClip.workspace",
                                                                          QJsonValue::Object);
                            workspace->setObjectAcceptAll(QsApi::AceTreeSerializer::DynamicData);
                            audioClip->addChild("workspace", workspace);
                        }
                    }
                }
            }
        }
        return root;
    }

    static QsApi::AceTreeControllerBuilder *createDspxControllerBuilder() {
        auto root = new QsApi::AceTreeControllerBuilder("root");
        root->addBuilder("root", &DspxRootController::staticMetaObject);
        return root;
    }

    DspxSpecPrivate::DspxSpecPrivate(Core::DspxSpec *q) : q(q) {
        serializer = nullptr;
        controllerBuilder = nullptr;
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

    QsApi::AceTreeSerializer *DspxSpec::serializer() const {
        if (!d->serializer)
            d->serializer = createDspxRootSerializer();
        return d->serializer;
    }

    QsApi::AceTreeControllerBuilder *DspxSpec::controllerBuilder() const {
        if (!d->controllerBuilder)
            d->controllerBuilder = createDspxControllerBuilder();
        return d->controllerBuilder;
    }

} // Core