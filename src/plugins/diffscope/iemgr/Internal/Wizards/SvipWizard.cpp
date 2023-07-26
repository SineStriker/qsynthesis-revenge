#include "SvipWizard.h"

#include <QApplication>
#include <QFile>

#include <QMDecoratorV2.h>

#include <QFileInfo>
#include <QNrbfStream.h>
#include <QtMath>

namespace IEMgr ::Internal {

    SvipWizard::SvipWizard(QObject *parent) : IWizardFactory("iemgr.SvipWizard", parent) {
        setCategory("score.XiaoIce");
        setDisplayName([]() { return tr("SVIP file"); });
        setDescription([]() { return tr("XStudio 2.0 project file."); });
        setDisplayCategory([]() { return QApplication::translate("IEMgr::WizardCategory", "XiaoIce"); });
    }

    SvipWizard::~SvipWizard() {
    }

    QString SvipWizard::filter(Feature feature) const {
        Q_UNUSED(feature)
        return QString("%1(%2)").arg(tr("XStudio Svip Files"), "*.svip") + ";;" + IWizardFactory::filter(feature);
    }

    IWizardFactory::Features SvipWizard::features() const {
        return {ImportProject | ExportProject};
    }

    bool SvipWizard::runWizard(Feature feature, const QString &path, QWidget *parent) {
        switch (feature) {
            case IWizardFactory::ImportProject: {
                QDspxModel model;
                if (!load(path, &model, parent))
                    return false;
                break;
            }
            case IWizardFactory::ExportProject: {
                break;
            }
            default:
                break;
        }
        return true;
    }
    bool SvipWizard::load(const QString &filename, QDspxModel *out, QWidget *parent) {
        auto loadProjectFile = [](const QString &filename, QNrbf::XSAppModel *model) {
            QFile file(filename);
            if (!file.open(QIODevice::ReadOnly)) {
                return false;
            }

            QByteArray bytes = file.readAll();
            file.close();

            QNrbfStream in(bytes);

            QString svipName;
            QString version;

            in >> svipName;
            in >> version;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
//            qDebug() << version;

//            QNrbf::XSAppModel xsProject;
            in >> *model;
            if (in.status() != QDataStream::Ok) {
                return false;
            }
            return true;
        };

        QNrbf::XSAppModel xsProject;
        if (!loadProjectFile(filename, &xsProject))
            return false;

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
        for (const auto &xsTempo : qAsConst(xsProject.tempoList)) {
            QDspx::Tempo tempo;
            tempo.pos = xsTempo.pos;
            tempo.value = xsTempo.tempo / 100.0;
            timeline.tempos.append(tempo);
        }

        // TimeSignatures
        int timeSigIndex = 0;
        int currentTime = 0;
        for (const auto &xsTimeSig : qAsConst(xsProject.beatList)) {
            QDspx::TimeSignature timeSig;
            int time;
            if (timeSigIndex == 0)
                time = 0;
            else {
                auto currentBarIndex = xsTimeSig.barIndex;
                auto &prevTimeSig = xsProject.beatList.at(timeSigIndex - 1);
                auto prevBarIndex = prevTimeSig.barIndex;
                auto prevNumerator = prevTimeSig.beatSize.x;
                auto prevDenominator = prevTimeSig.beatSize.y;
                currentTime += (currentBarIndex - prevBarIndex) * 1920 * (prevNumerator / prevDenominator);
                time = currentTime;
            }
            timeSigIndex++;
            timeSig.pos = time;
            timeSig.num = xsTimeSig.beatSize.x;
            timeSig.den = xsTimeSig.beatSize.y;
            timeline.timeSignatures.append(timeSig);
        }

        content.timeline = timeline;

        // Tracks
        QDspx::Track track;
        for (const auto &xsTrack : qAsConst(xsProject.trackList)) {
            //            qDebug() << osTrack->Title;
            track.name = xsTrack->name;

            QDspx::TrackControl trackControl;
            trackControl.gain = toDecibelGain(xsTrack->volume);
            trackControl.pan = xsTrack->pan;
            trackControl.mute = xsTrack->mute;
            trackControl.solo = xsTrack->solo;
            track.control = trackControl;

            switch (xsTrack->type()) {
                case QNrbf::XSITrack::Singing: {
                    auto xsSingingTrack = xsTrack.dynamicCast<QNrbf::XSSingingTrack>();
                    auto *singingClipPtr = new QDspx::SingingClip;
                    QSharedPointer<QDspx::SingingClip> singingClip(singingClipPtr);

                    int length = 0;
                    for (const auto &osNote : qAsConst(xsSingingTrack->noteList)) {
                        QDspx::Note note;
                        note.pos = osNote.startPos;
                        note.length = osNote.widthPos;
                        note.keyNum = osNote.keyIndex;
                        note.lyric = osNote.lyric;
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
                    break;
                }
                case QNrbf::XSITrack::Instrument: {
                    auto xsInstTrack = xsTrack.dynamicCast<QNrbf::XSInstrumentTrack>();
                    auto *audioClipPtr = new QDspx::AudioClip;
                    QSharedPointer<QDspx::AudioClip> audioClip(audioClipPtr);
                    QDspx::ClipTime clipTime;
                    clipTime.clipStart = xsInstTrack->OffsetInPos;
                    clipTime.start = 0;
                    // TODO: Decode audio file to obtain its length.
                    audioClip->time = clipTime;
                    audioClip->name = xsInstTrack->name;
                    audioClip->path = xsInstTrack->InstrumentFilePath;
                    track.clips.append(audioClip);
                } break;
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
    bool SvipWizard::save(const QString &filename, const QDspxModel &in, QWidget *parent) {
        return false;
    }
    double SvipWizard::toLinearVolume(const double &gain) {
        return gain >= 0 ? qMin(gain / (20 * log10(4)) + 1.0, 2.0) : qPow(10, gain / 20.0);
    }
    double SvipWizard::toDecibelGain(const double &volume) {
        if (volume > 0)
            return (float) qMax(20 * log10(volume > 0.01 ? volume : 0.01), -70.0);
        else
            return -70; //-infinity
    }
    double SvipWizard::log10(const double &x) {
        return qLn(x) / qLn(10);
    }

}