#include "QDspxModel.h"
#include "../3rdparty/QMidi/src/QMidiFile.h"
#include "../SVS/Utau/Utils/QUtaUtils.h"

#include <QDebug>
#include <QFile>
#include <QTextCodec>

QDspxModel::QDspxModel() {
}

QDspxModel::~QDspxModel() {
}

bool QDspxModel::fromMidi(const QString &filename, QDspxModel *out) {
    QMidiFile midi;

    if (!midi.load(filename)) {
        qDebug() << "Failed to read MIDI file!";
        return 0;
    }

    //时间类型、midi种类、四分音符ticks数、轨道数
    qint8 divType = midi.divisionType();
    qint8 midiFormat = midi.fileFormat();
    qint16 Resolution = midi.resolution();
    qint16 tracksCount = midi.tracks().size();

    //校验tracks数量、midi种类
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
    QList<QPair<int, double>> tempos;
    QList<QMidiEvent *> tempMap = midi.eventsForTrack(0);
    for (int j = 0; j < tempMap.size(); ++j) {
        QMidiEvent *e = tempMap.at(j);
        if (e->type() == QMidiEvent::Meta) {
            qDebug() << "Cmd:" << Qt::hex << e->number();
            if (e->number() == QMidiEvent::Tempo) {
                qDebug() << "Tempo:" << e->tempo();
            } else if (e->number() == QMidiEvent::TimeSignature) {
                qDebug() << "TimeSignature:" << e->tick() << e->number();
            } else {
                qDebug() << "Else:" << e->number();
            }
        }
    }

    //单轨数据：轨道名、起止tick、音高、歌词
    struct TrackInfo {
        QString Name;
        QList<qint32> NotePos;
        QList<qint32> noteEnd;
        QList<qint8> noteKeyNum;
        QList<QString> NoteLyric;
        QString pitchRange;
    };
    QList<TrackInfo> Tracks;

    //解析元数据
    for (int i = midiFormat; i < tracksCount; ++i) {
        QList<QMidiEvent *> list = midi.eventsForTrack(i);
        QString name = "";
        TrackInfo trackInfo;
        for (int j = 0; j < list.size(); ++j) {
            QMidiEvent *e = list.at(j);           
            //midi元事件
            switch (e->type()) {
                case QMidiEvent::Meta: {
                    if (e->number() == QMidiEvent::TrackName) {
                        name = QString::fromLocal8Bit(e->data());
                        trackInfo.Name.append(name);
                    } else if (e->number() == QMidiEvent::Lyric) {
                        trackInfo.NoteLyric.append(QString::fromLocal8Bit(e->data()));
                    }
                    break;
                }
                case QMidiEvent::NoteOn: {
                    trackInfo.NotePos.append(e->tick());
                    trackInfo.noteKeyNum.append(e->note());
                    break;
                }
                case QMidiEvent::NoteOff: {
                    trackInfo.noteEnd.append(e->tick());
                    break;
                }
                default: {

                }
            }
        }
        qint8 keyLow = 48;
        qint8 keyHigh = 60;
        QString low = QUtaUtils::ToneNumToToneName(keyLow);
        QString high = QUtaUtils::ToneNumToToneName(keyHigh);
        trackInfo.pitchRange = QString(low + "-" + high);
        Tracks.append(trackInfo);
    }
    
    for (int i = 0; i < Tracks.size(); ++i) {
        qDebug() << Tracks.at(i).Name <<  Tracks.at(i).pitchRange;
        for (int j = 0; j < Tracks.at(i).NotePos.size(); ++j) {
            qDebug() << Tracks.at(i).NotePos.at(j) 
                     << Tracks.at(i).noteEnd.at(j)
                     << Tracks.at(i).noteKeyNum.at(j)
                     << Tracks.at(i).NoteLyric.at(j);
        }
    }
    return false;
}
