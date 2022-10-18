#include "QMidiConverter.h"

#include "QMidiFile.h"

#include "Utau/Config/UtaConstants.h"
#include "Utau/QUtaUtils.h"

QMidiConverter::QMidiConverter(QObject *parent) : IUstConverter(parent) {
}

QMidiConverter::~QMidiConverter() {
}

bool QMidiConverter::load(const QString &filename, QUstFile *out,
                          IUstConverter::Callback callback) {
    QMidiFile midi;
    if (!midi.load(filename)) {
        return false;
    }

    int tracksCount = midi.tracks().size();
    if (tracksCount == 0) {
        return false;
    }

    QStringList trackNames;
    QList<int> trackNoteCounts;
    QList<QPoint> trackRanges;
    QStringList trackRangeStrings;
    QList<QPair<int, double>> tempos;

    for (int i = 0; i < tracksCount; ++i) {
        QList<QMidiEvent *> list = midi.eventsForTrack(i);
        QString name = "";
        QPoint range(127, 0);
        int count = 0;

        for (int j = 0; j < list.size(); ++j) {
            QMidiEvent *e = list.at(j);
            if (e->type() == QMidiEvent::Meta) {
                if (e->number() == QMidiEvent::TrackName) {
                    name = QString::fromLocal8Bit(e->data());
                } else if (e->number() == QMidiEvent::Tempo) {
                    tempos.append(qMakePair(e->tick(), e->tempo()));
                }
            } else if (e->type() == QMidiEvent::NoteOn) {
                count++;
                int note = e->note();
                if (range.x() > note) {
                    range.setX(note);
                }
                if (range.y() < note) {
                    range.setY(note);
                }
            }
        }

        trackNoteCounts.append(count);
        trackNames.append(name);
        trackRanges.append(range);
    }

    // Pitch range
    for (int i = 0; i < trackRanges.size(); ++i) {
        QString low = QUtaUtils::ToneNumToToneName(trackRanges[i].x());
        QString high = QUtaUtils::ToneNumToToneName(trackRanges[i].y());
        if (trackNoteCounts[i] == 0) {
            trackRangeStrings.append("");
        } else {
            trackRangeStrings.append(QString(low + "-" + high));
        }
    }

    QStringList titles;
    for (int i = 0; i < trackRanges.size(); ++i) {
        QString str = trackNames[i];

        str = str.isEmpty() ? QObject::tr("Track %1").arg(QString::number(i + 1)) : str;
        str += " (";
        str += QObject::tr("%1 notes").arg(trackNoteCounts.at(i));
        if (!trackRangeStrings.at(i).isEmpty()) {
            str += ", ";
            str += trackRangeStrings.at(i);
        }
        str += ")";

        titles.append(str);
    }

    // Open Selections
    SelectArguments args{
        QObject::tr("Import Midi"),
        QObject::tr("Tracks in file"),
        titles,
        true,
    };
    QList<bool> result = callback(args);

    // After Selection
    int track = -1;
    for (int i = 0; i < result.size(); ++i) {
        if (result.at(i)) {
            track = i;
            break;
        }
    }

    // Parse notes
    QList<QUstNote> temp;
    {
        int resolution = midi.resolution();
        QList<QPoint> ons, offs;
        QVector<QPair<int, QString>> lyrics;

        QList<QMidiEvent *> list = midi.eventsForTrack(track);
        for (int i = 0; i < list.size(); ++i) {
            QMidiEvent *e = list.at(i);
            if (e->type() == QMidiEvent::NoteOn) {
                ons.append(QPoint(e->tick(), e->note()));
            } else if (e->type() == QMidiEvent::NoteOff) {
                offs.append(QPoint(e->tick(), e->note()));
            } else if (e->type() == QMidiEvent::Meta) {
                if (e->number() == QMidiEvent::Lyric) {
                    lyrics.append(qMakePair(e->tick(), QString::fromLocal8Bit(e->data())));
                }
            }
        }

        int prevTick = 0;
        int prevPitch = 60;
        QMap<int, int> noteMap;
        for (int i = 0; i < ons.size(); ++i) {
            int start = ons.at(i).x();
            int pitch = ons.at(i).y();
            int len = (offs.size() > i) ? (offs.at(i).x() - start) : 480;
            int tick;
            int length;

            if (pitch > 107) {
                pitch = 107;
            } else if (pitch < 24) {
                pitch = 24;
            }

            // Too Short
            if (len < 15) {
                continue;
            }

            // Blank Interval
            if (prevTick < start) {
                int len2 = start - prevTick;
                if (len2 < 15) {
                    temp.back().length += len2;
                } else {
                    tick = double(prevTick) / resolution * 480;
                    length = double(len2) / resolution * 480;

                    Q_UNUSED(tick)
                    temp.append(QUstNote(prevPitch, length, "R"));
                }
            } else if (prevTick > start) {
                continue;
            }

            tick = double(start) / resolution * 480;
            length = double(len) / resolution * 480;

            Q_UNUSED(tick)
            temp.append(QUstNote(pitch, length));

            noteMap.insert(start, temp.size() - 1);
            prevTick = start + len;
            prevPitch = pitch;
        }

        // Change Lyrics
        for (int i = 0; i < lyrics.size(); ++i) {
            auto it = noteMap.find(lyrics.at(i).first);
            if (it == noteMap.end()) {
                continue;
            }
            temp[it.value()].lyric = lyrics.at(i).second;
        }

        // Change Tempo
        for (int i = 0; i < tempos.size(); ++i) {
            auto it = noteMap.find(tempos.at(i).first);
            if (it == noteMap.end()) {
                continue;
            }
            temp[it.value()].tempo = tempos.at(i).second;
        }

        // Update Tempo
        if (!temp.isEmpty()) {
            if (temp[0].tempo == QUtaUtils::NODEF_DOUBLE) {
                if (tempos.isEmpty()) {
                    temp[0].tempo = Utau::DEFAULT_VALUE_TEMPO;
                } else {
                    temp[0].tempo = tempos[0].second;
                }
            }
            out->sectionNotes = temp;
            out->sectionSettings.globalTempo = temp[0].tempo;
        }
    }

    out->sectionSettings.projectName = trackNames[track];
    return true;
}

bool QMidiConverter::save(const QString &filename, const QUstFile &in) {
    return false;
}
