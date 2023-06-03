#include "OpenSvipWizard.h"
#include "QFile"
#include "QJsonDocument"
#include "Svip/QSvipModel.h"
#include <QApplication>
#include <QFileInfo>
#include <QMDecoratorV2.h>
#include <QtMath>

namespace IEMgr ::Internal {

    OpenSvipWizard::OpenSvipWizard(QObject *parent) : IWizardFactory("iemgr.OpenSvipWizard", parent) {
        setCategory("score.OpenVPI");
        qIDec->installLocale(this, _LOC(OpenSvipWizard, this));
    }

    OpenSvipWizard::~OpenSvipWizard() {
    }

    void OpenSvipWizard::reloadStrings() {
        setDisplayName(tr("OpenSVIP model"));
        setDescription(tr("OpenSVIP converter intermediate model."));
        setDisplayCategory(QApplication::translate("IEMgr::WizardCategory", "OpenVPI"));
    }

    QString OpenSvipWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QString("%1(%2)").arg(tr("OpenSVIP Json Files"), "*.json") + ";;" + IWizardFactory::filter(feature);
    }

    IWizardFactory::Features OpenSvipWizard::features() const {
        return {ImportProject | ExportProject};
    }

    bool OpenSvipWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
        switch (feature) {
            case IWizardFactory::ImportProject: {
                QDspxModel model;
                if (!load(path, &model, parent)) {
                    return false;
                }
                qDebug() << model.content.tracks.size();
                break;
            }
            case IWizardFactory::ExportProject:
                break;
            default:
                break;
        }
        return false;
    }

    bool OpenSvipWizard::load(const QString &filename, QDspxModel *out, QWidget *parent) {
        auto loadProjectFile = [](const QString &filename, QJsonObject *jsonObj) {
            QFile loadFile(filename);
            if(!loadFile.open(QIODevice::ReadOnly))
            {
                qDebug() << "Failed to open project file";
                return false;
            }
            QByteArray allData = loadFile.readAll();
            loadFile.close();
            QJsonParseError err;
            QJsonDocument json = QJsonDocument::fromJson(allData, &err);
            if (err.error != QJsonParseError::NoError)
                return false;
            if(json.isObject()) {
                *jsonObj = json.object();
            }
            return true;
        };

        QJsonObject jsonObj;
        if(!loadProjectFile(filename, &jsonObj)) return false;
        auto osProject = QSvipModel::fromJsonObject(jsonObj);
        qDebug() << osProject.TrackList.size();

        QDspxModel model;

        // MetaData
        QDspx::Metadata metadata;
        metadata.version = "1.0.0";
        metadata.name = QFileInfo(filename).baseName();
        metadata.author = "";
        model.metadata = metadata;

        // Content
        QDspx::Content content;

        // Master
        QDspx::Master master;
        QDspx::Control control;
        control.gain = 0;
        control.mute = false;
        master.control = control;
        content.master = master;

        QDspx::Timeline timeline;

        // Tempos
        for (const auto &osTempo : qAsConst(osProject.SongTempoList)) {
            QDspx::Tempo tempo;
            tempo.pos = osTempo.Position;
            tempo.value = osTempo.BPM;
            timeline.tempos.append(tempo);
        }

        // TimeSignatures
        int timeSigIndex = 0;
        int currentTime = 0;
        for (const auto &osTimeSig : qAsConst(osProject.TimeSignatureList)) {
            QDspx::TimeSignature timeSig;
            int time;
            if (timeSigIndex == 0)
                time = 0;
            else {
                auto currentBarIndex = osTimeSig.BarIndex;
                auto &prevTimeSig = osProject.TimeSignatureList.at(timeSigIndex - 1);
                auto prevBarIndex = prevTimeSig.BarIndex;
                auto prevNumerator = prevTimeSig.Numerator;
                auto prevDenominator = prevTimeSig.Denominator;
                currentTime += (currentBarIndex - prevBarIndex) * 1920 * (prevNumerator / prevDenominator);
                time = currentTime;
            }
            timeSigIndex++;
            timeSig.pos = time;
            timeSig.num = osTimeSig.Numerator;
            timeSig.den = osTimeSig.Denominator;
            timeline.timeSignatures.append(timeSig);
        }

        content.timeline = timeline;

        // Tracks
        QDspx::Track track;
        for (const auto &osTrack : qAsConst(osProject.TrackList)) {
//            qDebug() << osTrack->Title;
            track.name = osTrack->Title;

            QDspx::TrackControl trackControl;
            trackControl.gain = toDecibelGain(osTrack->Volume);
            trackControl.pan = osTrack->Pan;
            trackControl.mute = osTrack->Mute;
            trackControl.solo = osTrack->Solo;
            track.control = trackControl;

            switch (osTrack->type()) {
                case QSvipModel::Track::Singing: {
                    auto osSingingTrack = osTrack.dynamicCast<QSvipModel::SingingTrack>();
                    auto *singingClipPtr = new QDspx::SingingClip;
                    QSharedPointer<QDspx::SingingClip> singingClip(singingClipPtr);

                    int length = 0;
                    for (const auto &osNote : qAsConst(osSingingTrack->NoteList)) {
                        QDspx::Note note;
                        note.pos = osNote.StartPos;
                        note.length = osNote.Length;
                        note.keyNum = osNote.KeyNumber;
                        note.lyric = osNote.Lyric;
                        // TODO: Phoneme Conversion
                        singingClip->notes.append(note);
                        length = note.pos + note.length;
                    }

                    QDspx::ClipTime clipTime;
                    clipTime.start = 0;
                    clipTime.length = length;
                    clipTime.clipStart = 0;
                    clipTime.clipLen = length;
                    singingClip->time = clipTime;
                    // TODO: Params Conversion
                    singingClip->name = "Pattern";
                    track.clips.append(singingClip);
//                    QString lyrics;
//                    for (const auto &note : qAsConst(osSingingTrack->NoteList)) {
//                        lyrics.append(note.Lyric);
//                    }
//                    qDebug() << lyrics;
                    break;
                }
                case QSvipModel::Track::Instrumental: {
                    auto osInstTrack = osTrack.dynamicCast<QSvipModel::InstrumentalTrack>();
                    auto *audioClipPtr = new QDspx::AudioClip;
                    QSharedPointer<QDspx::AudioClip> audioClip(audioClipPtr);
                    QDspx::ClipTime clipTime;
                    clipTime.clipStart = osInstTrack->Offset;
                    clipTime.start = 0;
                    // TODO: Decode audio file to obtain its length.
                    audioClip->time = clipTime;
                    audioClip->name = osInstTrack->Title;
                    audioClip->path = osInstTrack->AudioFilePath;
                    track.clips.append(audioClip);
                }
                    break;
                default:
                    break;
            }
            content.tracks.append(track);
        }

        model.content = content;
        *out = model;
        model.save("D:/test.dspx");
        return true;
    }

    bool OpenSvipWizard::save(const QString &filename, const QDspxModel &in, QWidget *parent) {
        auto &dsProject = in;
        auto &dsContent = dsProject.content;
        auto &dsTimeline = dsContent.timeline;

        QSvipModel model;
        
        // Tempos
        for (const auto &dsTempo : qAsConst(dsTimeline.tempos)) {
            QSvipModel::SongTempo tempo;
            tempo.Position = dsTempo.pos;
            tempo.BPM = dsTempo.value;
            model.SongTempoList.append(tempo);
        }

        // TimeSignatures
        int timeSigIndex = 0;
        int currentBarIndex = 0;
        for (const auto &dsTimeSig : qAsConst(dsTimeline.timeSignatures)) {
            int deltaTime = 0;
            int barIndex;
            if (timeSigIndex == 0)
                barIndex = 0;
            else {
                auto curTime = dsTimeSig.pos;
                auto &prevTimeSig = dsTimeline.timeSignatures.at(timeSigIndex - 1);
                auto prevTime = prevTimeSig.pos;
                deltaTime = curTime - prevTime;
                auto bars = deltaTime * prevTimeSig.den / 1920 / prevTimeSig.num;
                currentBarIndex += bars;
                barIndex = currentBarIndex;
            }
            timeSigIndex++;
            QSvipModel::TimeSignature timeSig;
            timeSig.BarIndex = barIndex;
            timeSig.Numerator = dsTimeSig.num;
            timeSig.Denominator = dsTimeSig.den;
            model.TimeSignatureList.append(timeSig);
        }

        // Tracks
        for (const auto &dsTrack : qAsConst(dsContent.tracks)) {
            if (dsTrack.clips.isEmpty()) { // Insert an empty singing track if ds track is empty.
                auto *singingTrackPtr = new QSvipModel::SingingTrack;
                QSharedPointer<QSvipModel::SingingTrack> singingTrack(singingTrackPtr);
                singingTrack->Title = dsTrack.name;
                singingTrack->Mute = dsTrack.control.mute;
                singingTrack->Solo = dsTrack.control.solo;
                singingTrack->Volume = OpenSvipWizard::toLinearVolume(dsTrack.control.gain);
                singingTrack->Pan = dsTrack.control.pan;
                model.TrackList.append(singingTrack);
            } else { // Assign clips to proper tracks
                for (const auto &dsClip : qAsConst(dsTrack.clips)) {
                    if (dsClip->type == QDspx::Clip::Singing) {
                        auto dsSingingClip = dsClip.dynamicCast<QDspx::SingingClip>();
                        auto *singingTrackPtr = new QSvipModel::SingingTrack;
                        QSharedPointer<QSvipModel::SingingTrack> singingTrack(singingTrackPtr);
                        singingTrack->Title = dsTrack.name + " " + dsSingingClip->name;
                        singingTrack->Mute = dsSingingClip->control.mute;
                        singingTrack->Solo = dsTrack.control.solo;
                        singingTrack->Volume = OpenSvipWizard::toLinearVolume(dsSingingClip->control.gain);
                        singingTrack->Pan = dsTrack.control.pan;

                        // Only visiable notes will be exported
                        int start = dsClip->time.start;
                        int clipStart = dsClip->time.clipStart;
                        int visiableLeft = start + clipStart;
                        int length = dsClip->time.length;
                        int clipLen = dsClip->time.clipLen;
                        int visiableRight = visiableLeft + clipLen;
                        for (const auto &dsNote : qAsConst(dsSingingClip->notes)) {
                            if (dsNote.pos < visiableLeft || dsNote.pos + dsNote.length > visiableRight) // Ignore invisible note
                                continue;

                            QSvipModel::Note note;
                            note.StartPos = dsNote.pos;
                            note.Length = dsNote.length;
                            note.Lyric = dsNote.lyric;
                            note.KeyNumber = dsNote.keyNum;
                            // TODO: Phoneme conversion
                            singingTrack->NoteList.append(note);
                        }
                        // TODO: Params Conversion
                        model.TrackList.append(singingTrack);
                    } else if (dsClip->type == QDspx::Clip::Audio) {
                        auto dsAudioClip = dsClip.dynamicCast<QDspx::AudioClip>();
                        auto *instTrackPtr = new QSvipModel::InstrumentalTrack;
                        QSharedPointer<QSvipModel::InstrumentalTrack> instTrack(instTrackPtr);
                        instTrack->Title = dsTrack.name + " " + dsAudioClip->name;
                        instTrack->Mute = dsAudioClip->control.mute;
                        instTrack->Solo = dsTrack.control.solo;
                        instTrack->Volume = OpenSvipWizard::toLinearVolume(dsAudioClip->control.gain);
                        instTrack->Pan = dsTrack.control.pan;

                        instTrack->AudioFilePath = dsAudioClip->path;
                        instTrack->Offset = dsAudioClip->time.start;
                        model.TrackList.append(instTrack);
                    }
                }
            }
        }

        auto saveProjectFile = [](const QString &filename, QJsonObject &jsonObj) -> bool {
            QJsonDocument document;
            document.setObject(jsonObj);

            auto byteArray = document.toJson(QJsonDocument::Compact);
            QString jsonStr(byteArray);
            QFile file(filename);

            if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
                qDebug() << "Failed to write project file";
                return false;
            }
            QTextStream in(&file);
            in << jsonStr;

            file.close();
            return true;
        };

        auto modelObj = model.toJsonObject();
        if (!saveProjectFile(filename, modelObj))
            return false;
        return true;
    }

    double OpenSvipWizard::toLinearVolume(const double &gain) {
        return gain >= 0
                   ? qMin(gain / (20 * log10(4)) + 1.0, 2.0)
                   : qPow(10, gain / 20.0);
    }

    double OpenSvipWizard::toDecibelGain(const double &volume) {
        if (volume > 0)
            return (float)qMax(20 * log10(volume > 0.01 ? volume : 0.01), -70.0);
        else
            return -70;//-infinity
    }

    double OpenSvipWizard::log10(const double &x) {
        return qLn(x) / qLn(10);
    }

}