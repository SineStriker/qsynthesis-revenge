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
                auto prevTimeSig = osProject.TimeSignatureList.at(timeSigIndex - 1);
                auto prevBarIndex = prevTimeSig.BarIndex;
                auto prevNumerator = prevTimeSig.Numerator;
                auto prevDenominator = prevTimeSig.Denominator;
                currentTime += (currentBarIndex - prevBarIndex) * 1920 * (prevNumerator / prevDenominator);
                time = currentTime;
            }
            timeSigIndex += 1;
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