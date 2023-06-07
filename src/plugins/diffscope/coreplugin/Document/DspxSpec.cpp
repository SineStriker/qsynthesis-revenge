#include "DspxSpec.h"
#include "DspxSpec_p.h"

#include <QCoreApplication>
#include <QDebug>
#include <QMessageBox>
#include <QTimer>

#include <QMDecoratorV2.h>

#include "ICore.h"
#include "Window/IProjectWindow.h"

namespace Core {

    DspxSpecPrivate::DspxSpecPrivate(Core::DspxSpec *q) : q(q) {
    }

    DspxSpecPrivate::~DspxSpecPrivate() {
    }

    void DspxSpecPrivate::init() {
        auto root = new QsApi::AceTreeSerializer("root", QJsonValue::Object);
        serializer = root;

        // metadata
        {
            auto metadata = new QsApi::AceTreeSerializer("root.metadata");
            metadata->setObjectAcceptAll(QsApi::AceTreeSerializer::Property);
            metadata->setObjectAcceptOnes({
                {"version", QsApi::AceTreeSerializer::DynamicData},
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
                        {"gain", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                        {"mute", {QsApi::AceTreeSerializer::Property, QJsonValue::Bool}  },
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
                        new QsApi::AceTreeSerializer("root.content.timeline.timeSignatures", QJsonValue::Object);
                    timeSignatures->setObjectAcceptOnes({
                        {"pos",         {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                        {"numerator",   {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                        {"denominator", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                    });
                    timeline->addChild("timeSignatures", timeSignatures);
                }

                // tempos
                {
                    auto tempos = new QsApi::AceTreeSerializer("root.content.timeline.tempos", QJsonValue::Object);
                    tempos->setObjectAcceptOnes({
                        {"pos",   {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                        {"value", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                    });
                    timeline->addChild("tempos", tempos);
                }

                // labels
                {
                    auto labels = new QsApi::AceTreeSerializer("root.content.timeline.labels", QJsonValue::Object);
                    labels->setObjectAcceptOnes({
                        {"pos",   {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                        {"value", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                    });
                    timeline->addChild("labels", labels);
                }
            }

            // tracks
            {
                auto tracks = new QsApi::AceTreeSerializer("root.content.tracks", QJsonValue::Array);
                content->addChild("tracks", tracks);

                auto track = new QsApi::AceTreeSerializer("root.content.track", QJsonValue::Object);
                track->setObjectAcceptOnes({
                    {"name", {QsApi::AceTreeSerializer::Property, QJsonValue::String}},
                });
                tracks->addChild("item", track);

                // control
                {
                    auto control = new QsApi::AceTreeSerializer("root.content.track.control", QJsonValue::Object);
                    control->setObjectAcceptOnes({
                        {"gain", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                        {"pan",  {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                        {"mute", {QsApi::AceTreeSerializer::Property, QJsonValue::Bool}  },
                        {"solo", {QsApi::AceTreeSerializer::Property, QJsonValue::Bool}  },
                    });
                    track->addChild("control", control);
                }

                // clip
                {
                    auto clips = new QsApi::AceTreeSerializer("root.content.track.clips", QJsonValue::Array);
                    clips->setArrayTypeField("type");
                    track->addChild("clips", clips);

                    // singing
                    auto singingClip =
                        new QsApi::AceTreeSerializer("root.content.track.singingClip", QJsonValue::Object);
                    singingClip->setObjectAcceptOnes({
                        {"name", {QsApi::AceTreeSerializer::Property, QJsonValue::String}},
                    });
                    clips->addChild("singing", singingClip);

                    // time
                    {
                        auto time =
                            new QsApi::AceTreeSerializer("root.content.track.singingClip.time", QJsonValue::Object);
                        time->setObjectAcceptOnes({
                            {"start",     {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"length",    {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"clipStart", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"clipLen",   {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                        });
                        singingClip->addChild("time", time);
                    }

                    // control
                    {
                        auto control =
                            new QsApi::AceTreeSerializer("root.content.track.singingClip.control", QJsonValue::Object);
                        control->setObjectAcceptOnes({
                            {"gain", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"mute", {QsApi::AceTreeSerializer::Property, QJsonValue::Bool}  },
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
                            {"pos",    {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"length", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"keyNum", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"lyric",  {QsApi::AceTreeSerializer::Property, QJsonValue::String}},
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
                            auto vibrato = new QsApi::AceTreeSerializer("root.content.track.singingClip.note.vibrato",
                                                                        QJsonValue::Object);
                            vibrato->setObjectAcceptOnes({
                                {"start",  {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                {"end",    {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                {"freq",   {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                {"phase",  {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                {"amp",    {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                {"offset", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
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
                                    {"x", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                    {"y", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                });
                                points->addChild("point", point);
                            }
                        }

                        // phonemes
                        {
                            auto phonemes = new QsApi::AceTreeSerializer("root.content.track.singingClip.note.phonemes",
                                                                         QJsonValue::Object);
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
                                {"type",  {QsApi::AceTreeSerializer::Property, QJsonValue::String}},
                                {"token", {QsApi::AceTreeSerializer::Property, QJsonValue::String}},
                                {"start", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            });
                            edited->addChild("item", phoneme);
                        }

                        // params
                        {
                            auto params = new QsApi::AceTreeSerializer("root.content.track.singingClip.note.params",
                                                                       QJsonValue::Object);
                            note->addChild("params", params);

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
                                        {"x",      {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                        {"y",      {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                        {"interp", {QsApi::AceTreeSerializer::Property, QJsonValue::String}},
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
                                         {"start", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                                         {"step", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
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
                                            return QsApi::AceTreeSerializer::Success;
                                         },

                                        [](const QString &key, QJsonObject *obj,
                                           const QsApi::AceTreeSerializer::ChildOrProperty &cop, void *) {
                                            if (key != "values")
                                                return QsApi::AceTreeSerializer::Unhandled;
                                            if (!cop.is_item) {
                                                return QsApi::AceTreeSerializer::Failed;
                                            }

                                            QJsonArray arr;
                                            QByteArray bytes = cop.i->bytes();
                                            QDataStream stream(&bytes, QIODevice::ReadOnly);
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
                                    });
                                    edited->addChild("free", free);
                                }
                            }
                        }
                    }

                    // audio
                    auto audioClip = new QsApi::AceTreeSerializer("root.content.track.audioClip", QJsonValue::Object);
                    audioClip->setObjectAcceptOnes({
                        {"name", {QsApi::AceTreeSerializer::Property, QJsonValue::String}},
                        {"path", {QsApi::AceTreeSerializer::Property, QJsonValue::String}},
                    });
                    clips->addChild("audio", audioClip);

                    // time
                    {
                        auto time =
                            new QsApi::AceTreeSerializer("root.content.track.audioClip.time", QJsonValue::Object);
                        time->setObjectAcceptOnes({
                            {"start",     {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"length",    {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"clipStart", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"clipLen",   {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                        });
                        audioClip->addChild("time", time);
                    }

                    // control
                    {
                        auto control =
                            new QsApi::AceTreeSerializer("root.content.track.audioClip.control", QJsonValue::Object);
                        control->setObjectAcceptOnes({
                            {"gain", {QsApi::AceTreeSerializer::Property, QJsonValue::Double}},
                            {"mute", {QsApi::AceTreeSerializer::Property, QJsonValue::Bool}  },
                        });
                        audioClip->addChild("control", control);
                    }

                    // workspace
                    {
                        auto workspace =
                            new QsApi::AceTreeSerializer("root.content.track.audioClip.workspace", QJsonValue::Object);
                        workspace->setObjectAcceptAll(QsApi::AceTreeSerializer::DynamicData);
                        audioClip->addChild("workspace", workspace);
                    }
                }
            }
        }
    }

    void DspxSpecPrivate::reloadStrings() {
        q->setDisplayName(tr("OpenVPI DiffSinger Editor"));
        q->setDescription(tr("Built-in"));
    }

    DspxSpec *m_instance = nullptr;

    DspxSpec::DspxSpec(QObject *parent) : DocumentSpec("org.ChorusKit.dspx", parent), d(new DspxSpecPrivate(this)) {
        m_instance = this;

        d->init();

        setIcon(QIcon(":/images/dspx.png"));
        qIDec->installLocale(this, _LOC(DspxSpecPrivate, d));
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
        return d->serializer;
    }

} // Core