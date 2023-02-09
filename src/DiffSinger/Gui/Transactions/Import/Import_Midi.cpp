#include "../ProjectImport.h"

#include "QMidiFile.h"
#include "Utau/Utils/QUtaUtils.h"

#include "Dialogs/ImportDialog.h"

#include <QChar>
#include <QDebug>
#include <QFile>
#include <QTextCodec>

#include <set>

using namespace QDspx;

bool Import::loadMidi(const QString &filename, QDspxModel *out, QObject *parent) {
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
    QVector<QPair<int, QByteArray>> marker;

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
                    marker.append(qMakePair(e->tick(), data));
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

#pragma pack(1)
    struct LogicTrack {
        quint8 key;     // 0~16
        quint8 channel; // 16~24
        quint16 track;  // 24~32

        LogicTrack() : LogicTrack(0, 0, 0){};
        LogicTrack(quint16 track, quint8 channel, quint8 key)
            : track(track), channel(channel), key(key){};

        [[nodiscard]] qint32 toInt() const {
            return *reinterpret_cast<const qint32 *>(this);
        }

        static LogicTrack fromInt(qint32 n) {
            return *reinterpret_cast<LogicTrack *>(&n);
        }
    };
#pragma pack()

    Q_STATIC_ASSERT(sizeof(LogicTrack) == sizeof(qint32));

    struct TrackNameAndLyrics {
        QByteArray name;
        QMap<qint32, QByteArray> lyrics; // key: pos; value: lyric;
    };

    // key: pack(track, channel, 0); ordered
    std::set<qint32> trackAndChannelIndexSet;

    // key: track index; value: track name;
    QHash<qint32, TrackNameAndLyrics> trackNameAndLyrics;

    // key: pack(track, channel, key);  value: ...;
    QHash<qint32, QPair<QList<qint32>, QList<qint32>>> noteMap;

    // 解析元数据
    for (int i = midiFormat; i < tracksCount; ++i) {
        QList<QMidiEvent *> list = midi.eventsForTrack(i);
        qint32 trackIndex = i - midiFormat + 1;

        TrackNameAndLyrics cur;

        // 以track、channel、note为索引打包数据
        for (auto e : list) {
            // midi元事件
            switch (e->type()) {
                case QMidiEvent::Meta: {
                    switch (e->number()) {
                        case QMidiEvent::TrackName:
                            cur.name = e->data();
                            break;
                            //                        case (QMidiEvent::MetaNumbers) 0x4:
                            //                            qDebug() << "##################" <<
                            //                            QString::fromLocal8Bit(e->data());
                            //                            cur.instrument = e->data();
                            //                            break;
                        case QMidiEvent::Lyric:
                            cur.lyrics.insert(e->tick(), e->data());
                            break;
                        default:
                            break;
                    }
                    break;
                }
                case QMidiEvent::NoteOn: {
                    // Add packed(track, channel, 0)
                    trackAndChannelIndexSet.insert(LogicTrack(trackIndex, e->voice(), 0).toInt());
                    // Add packed(track, channel, key)
                    noteMap[LogicTrack(trackIndex, e->voice(), e->note()).toInt()].first.append(
                        e->tick());
                    break;
                }
                case QMidiEvent::NoteOff: {
                    // Add packed(track, channel, key)
                    noteMap[LogicTrack(trackIndex, e->voice(), e->note()).toInt()].second.append(
                        e->tick());
                    break;
                }
                default:
                    break;
            }
        }

        // Add
        trackNameAndLyrics.insert(trackIndex, cur);
    }

    struct LogicTrackInfo {
        ImportDialog::TrackInfo option;
    };

    // 解析轨道
    // key: pack(track, channel, 0); value: ...;
    QMap<qint32, LogicTrackInfo> logicTrackInfos;

    struct LogicNote {
        int pos;
        int len;
        int key;
        QByteArray lyric;

        LogicNote() : LogicNote(0, 0, 0){};
        LogicNote(int pos, int len, int key) : pos(pos), len(len), key(key) {
        }

        bool operator<(const LogicNote &other) const {
            if (pos == other.pos) {
                return key < other.key;
            }
            return pos < other.pos;
        }
    };

    // key: pack(track, channel, 0); value: (pos, LogicNote)
    QMap<qint32, std::set<LogicNote>> logicTrackNotes;

    // 现存逻辑轨道
    for (const auto &packData : qAsConst(trackAndChannelIndexSet)) {
        LogicTrack trackAndChannelIndex = LogicTrack::fromInt(packData);

        qint32 noteCount = 0;
        std::set<qint32> staticKeyNum;

        auto &currentNoteSet = logicTrackNotes[packData];

        // 遍历keyNum
        for (int key = 0; key < 128; ++key) {
            LogicTrack tempIndex(trackAndChannelIndex.track, trackAndChannelIndex.channel, key);

            auto it = noteMap.find(tempIndex.toInt());
            if (it == noteMap.end()) {
                // 当前trackAndChannelIndex的当前key没有音符
                continue;
            }

            // 以track,channel,note为索引，取出noteMap
            const auto &noteListPair = it.value();

            // 校验各keyNum的NoteOn/Off事件配对
            if (noteListPair.first.size() != noteListPair.second.size()) {
                qDebug() << "The number of note-on and note-off are not match";
                return false;
            }

            // 存储出现的key
            staticKeyNum.insert(key);

            // 逻辑轨道封装乱序Note
            for (int i = 0; i < noteListPair.first.size(); ++i) {
                noteCount++;

                LogicNote note;
                note.pos = noteListPair.first[i];
                note.len = noteListPair.second[i] - note.pos;
                note.key = key;

                {
                    const auto &lyricsMap = trackNameAndLyrics[trackAndChannelIndex.track].lyrics;
                    auto it2 = lyricsMap.find(note.pos);
                    if (it2 != lyricsMap.end()) {
                        note.lyric = it2.value();
                    }
                }

                currentNoteSet.insert(note);
            }
        }

        // 获取逻辑轨道音域
        QString logicTrackPitchRange;
        if (!staticKeyNum.empty()) {
            logicTrackPitchRange = QUtaUtils::ToneNumToToneName(*staticKeyNum.begin()) + "-" +
                                   QUtaUtils::ToneNumToToneName(*staticKeyNum.rbegin());
        }


        // 逻辑轨道名称
        auto nameBytes = trackNameAndLyrics[trackAndChannelIndex.track].name;
        ImportDialog::TrackInfo info(
            nameBytes, trackNameAndLyrics[trackAndChannelIndex.track].lyrics.values());

        info.format =
            QObject::tr("%1(%2): (%3 notes, %4)")
                .arg("%1", QString::number(trackAndChannelIndex.channel),
                     QString::number(noteCount),
                     logicTrackPitchRange.isEmpty() ? QObject::tr("None") : logicTrackPitchRange);

        logicTrackInfos.insert(packData, LogicTrackInfo{info});

        qDebug() << trackAndChannelIndex.track << trackAndChannelIndex.channel;
    }


    QList<qint32> selectID;
    QTextCodec *codec = nullptr;

    // 获取选中轨道
    {
        ImportDialog dlg(qobject_cast<QWidget *>(parent));
        dlg.setWindowTitle(QObject::tr("Import MIDI file"));

        ImportDialog::ImportOptions opt;
        opt.maxTracks = 32;

        QList<qint32> logicIndexList;
        for (auto it = logicTrackInfos.begin(); it != logicTrackInfos.end(); ++it) {
            logicIndexList.append(it.key());
            opt.tracks.append(it.value().option);
        }
        dlg.setImportOptions(opt);
        if (dlg.exec() == 0) {
            return false;
        }
        codec = dlg.codecResult();

        auto selectResult = dlg.selectResult();
        qDebug() << "select res" << selectResult;

        for (auto index : qAsConst(selectResult)) {
            selectID.append(logicIndexList[index]);
        }
        for (const auto &id : qAsConst(selectID)) {
            auto info = LogicTrack::fromInt(id);
            qDebug() << info.track << info.channel << info.key;
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

    // Track数据
    // QHash<qint32, QHash<qint32, QDspx::Track>> logicTracks;
    //    QDspx::Track track;
    //
    //    for (auto &trackID : selectID) {
    //        auto clip = SingingClipRef::create();
    //        TrackInfo trackInfo = trackInfos[trackID];
    //        for (int i = 0; i < trackInfo.notePos.size(); ++i) {
    //            Note note;
    //            note.pos = trackInfo.notePos[i];
    //            // clip->notes.append();
    //        }
    //
    //        track.clips.append(clip);
    //        out->content.tracks.append(track);
    //    }





    return true;
}
