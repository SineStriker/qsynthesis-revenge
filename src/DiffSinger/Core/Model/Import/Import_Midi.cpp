#include "../QDspxModel.h"

#include "QMidiFile.h"
#include "Utau/Utils/QUtaUtils.h"

#include <QChar>
#include <QDebug>
#include <QFile>
#include <QTextCodec>

bool QDspx::fromMidi(const QString &filename, QDspxModel *out, QObject *parent) {
    QMidiFile midi;

    if (!midi.load(filename)) {
        qDebug() << "Failed to read MIDI file!";
        return false;
    }

    // midi种类、四分音符ticks数、轨道数、时间类型
    int midiFormat = midi.fileFormat();
    int resolution = midi.resolution();
    qDebug() << "resolution:" << resolution;
    int tracksCount = midi.tracks().size();
    int divType = midi.divisionType();

    // 校验tracks数量、midi种类
    if (tracksCount == 0) {
        qDebug() << "The midi is empty!";
        return false;
    } else if (midiFormat == 0 && tracksCount > 1) {
        qDebug() << "The number of midi tracks is illegal!";
        return false;
    } else if (midiFormat == 2 || divType != QMidiFile::PPQ) {
        qDebug() << "MidiFormat:" << midiFormat;
        qDebug() << "DivType:" << divType << midi.divisionType();
        qDebug() << "The midi file is illegal!";
        return false;
    }

    // 解析Tempo Map
    QVector<QPair<int, double>> tempos;
    QVector<QPair<int, QString>> marker;
    QMap<int, QPoint> timeSign;
    timeSign[0] = QPoint(4, 4);
    QList<QMidiEvent *> tempMap = midi.eventsForTrack(0);

    for (auto e : qAsConst(tempMap)) {
        const auto &data = e->data();
        if (e->type() == QMidiEvent::Meta) {
            qDebug() << "Cmd:" << Qt::hex << e->number();
            switch (e->number()) {
                case QMidiEvent::Tempo:
                    tempos.append(qMakePair(e->tick(), e->tempo()));
                    qDebug() << "Tempo:" << e->tick() << e->tempo();
                    break;
                case QMidiEvent::Marker:
                    marker.append(qMakePair(e->tick(), QString(data)));
                    qDebug() << "Marker:" << e->tick() << QString(data);
                    break;
                case QMidiEvent::TimeSignature:
                    timeSign[e->tick()] = QPoint(data[0], 2 * data[1]);
                    qDebug() << "TimeSignature:" << e->tick() << timeSign[e->tick()];
                    break;
                default:
                    qDebug() << "Else:" << e->number();
                    break;
            }
        }
    }

    // 单轨数据：轨道名、起止tick、音高、歌词
    struct TrackInfo {
        QString name;
        QList<qint32> notePos;
        QList<qint32> noteEnd;
        QList<qint32> noteKeyNum;
        QList<qint32> noteChannel;
        QList<QPair<qint32, QString>> noteLyric;
        QString pitchRange;
    };

    QMap<qint32, TrackInfo> trackInfos;
    QMap<qint32, QString> trackTitles;

    // 解析元数据
    for (int i = midiFormat; i < tracksCount; ++i) {
        QList<QMidiEvent *> list = midi.eventsForTrack(i);
        QString name;
        TrackInfo trackInfo;
        qint32 trackID = i - midiFormat + 1;
        for (auto e : list) {
            // midi元事件
            switch (e->type()) {
                case QMidiEvent::Meta: {
                    if (e->number() == QMidiEvent::TrackName) {
                        name = QString::fromLocal8Bit(e->data());
                    } else if (e->number() == QMidiEvent::Lyric) {
                        trackInfo.noteLyric.append(qMakePair(e->tick(), QString::fromLocal8Bit(e->data())));
                    }
                    break;
                }
                case QMidiEvent::NoteOn: {
                    trackInfo.notePos.append(e->tick());
                    trackInfo.noteKeyNum.append(e->note());
                    trackInfo.noteChannel.append(e->voice());
                    break;
                }
                case QMidiEvent::NoteOff: {
                    trackInfo.noteEnd.append(e->tick());
                    break;
                }
                default:
                    break;
            }
        }

        if (trackInfo.notePos.size() != trackInfo.noteEnd.size()) {
            qDebug() << "The number of note-on and note-off are not match";
            return false;
        }

        if (!trackInfo.noteKeyNum.empty()) {
            qint32 keyLow =
                *std::min_element(trackInfo.noteKeyNum.begin(), trackInfo.noteKeyNum.end());
            qint32 keyHigh =
                *std::max_element(trackInfo.noteKeyNum.begin(), trackInfo.noteKeyNum.end());
            QString low = QUtaUtils::ToneNumToToneName(keyLow);
            QString high = QUtaUtils::ToneNumToToneName(keyHigh);
            trackInfo.pitchRange = low + "-" + high;
        } else {
            trackInfo.pitchRange.clear();
        }
        trackInfo.name =
            QObject::tr("Track %1").arg(name.isEmpty() ? QString::number(trackID) : name);
        trackTitles[trackID] =
            QObject::tr("%1: (%2 notes, %3)")
                .arg(trackInfo.name, QString::number(trackInfo.noteKeyNum.size()),
                     trackInfo.pitchRange);
        trackInfos[trackID] = trackInfo;
    }

    qDebug() << timeSign << trackTitles;
    for (const auto &info : trackInfos) {
        for (int j = 0; j < info.notePos.size(); ++j) {
            qDebug() << info.notePos.at(j) << info.noteEnd.at(j) << info.noteKeyNum.at(j)
                     << info.noteChannel.at(j);
                     //<< (info.noteLyric.size() > j ? info.noteLyric.at(j) : "");
        }
    }

    // 缩放系数
    double scaleFactor = 480.0 / resolution;

    // TempoMap轨道数据
    QDspx::Timeline timeLine;

    TimeSignature timeSignature;
    for (auto it = timeSign.begin(); it != timeSign.end(); ++it) {
        timeSignature.pos = int(it.key() * scaleFactor);
        timeSignature.num = it->x();
        timeSignature.den = it->y();
        timeLine.timeSignatures.append(timeSignature);
    }

    QDspx::Tempo tempo;
    for (auto &it : tempos) {
        tempo.pos = int(it.first * scaleFactor);
        tempo.value = it.second;
        timeLine.tempos.append(tempo);
    }

    QDspx::Label label;
    for (auto &it : marker) {
        label.pos = int(it.first * scaleFactor);
        label.text = it.second;
        timeLine.labels.append(label);
    }

    out->content.timeline = timeLine;

    // 获取选中轨道
    QVector<qint32> selectID;
    selectID.append(1);

    // Track数据
    QDspx::Track track;

    for (auto &trackID : selectID) {
        auto clip = SingingClipRef::create();
        TrackInfo trackInfo = trackInfos[trackID];
        for (int i = 0; i < trackInfo.notePos.size(); ++i) {
            Note note;
            note.pos = trackInfo.notePos[i];
            //clip->notes.append();
        }

        track.clips.append(clip);
        out->content.tracks.append(track);
    }

    return true;
}
