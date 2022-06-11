#include "../FileParser.h"
#include "QMidiFile.h"
#include "QUtauSections.h"

using namespace Utau;

bool FileParser::parseMidiFile(const QString &filename, SectionNotes &oNotes) {
    QMidiFile midi;
    if (!midi.load(filename)) {
//        QMessageBox::warning(Q_W(parent()), qData->MainTitle, tr("Unable to read MIDI file!"));
        return 0;
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
        QString low = Utau::ToneNumToToneName(trackRanges[i].x());
        QString high = Utau::ToneNumToToneName(trackRanges[i].y());
        if (trackNoteCounts[i] == 0) {
            trackRangeStrings.append("");
        } else {
            trackRangeStrings.append(QString(low + "-" + high));
        }
    }

    QStringList titles;
    for (int i = 0; i < trackRanges.size(); ++i) {
        QString str = trackNames[i];

        str = str.isEmpty() ? tr("Track %1").arg(QString::number(i + 1)) : str;
        str += " (";
        str += tr("%1 notes").arg(trackNoteCounts.at(i));
        if (!trackRangeStrings.at(i).isEmpty()) {
            str += ", ";
            str += trackRangeStrings.at(i);
        }
        str += ")";

        titles.append(str);
    }

    QList<bool> result;
    //    auto dlg = new SelectsDialogV2(tr("Import Midi"), tr("Tracks in file"), titles, result,
    //    true,
    //                                   Q_W(parent()));

    //    int code = dlg->exec();
    //    dlg->deleteLater();
    int code = 0;

    if (code != 1) {
        return false;
    }

    int track = -1;
    for (int i = 0; i < result.size(); ++i) {
        if (result.at(i)) {
            track = i;
            break;
        }
    }

    // Parse notes
    QList<QLinkNote> notes;
    {
        int resolution = midi.resolution();
        QList<QPoint> noteOns, noteOffs;
        QList<QPair<int, QString>> lyrics;

        QList<QMidiEvent *> list = midi.eventsForTrack(track);
        for (int i = 0; i < list.size(); ++i) {
            QMidiEvent *e = list.at(i);
            if (e->type() == QMidiEvent::NoteOn) {
                noteOns.append(QPoint(e->tick(), e->note()));
            } else if (e->type() == QMidiEvent::NoteOff) {
                noteOffs.append(QPoint(e->tick(), e->note()));
            } else if (e->type() == QMidiEvent::Meta) {
                if (e->number() == QMidiEvent::Lyric) {
                    lyrics.append(qMakePair(e->tick(), QString::fromLocal8Bit(e->data())));
                }
            }
        }

        int prevStart = 0;
        int prevEnd = 0;
        int prevPitch = 60;
        QMap<int, int> noteMap;

        for (int i = 0; i < noteOns.size(); ++i) {
            int start = noteOns.at(i).x();
            int end = (noteOffs.size() > i) ? noteOffs.at(i).x() : (start + 480);

            int pitch = noteOns.at(i).y();
            int len = end - start;

            if (pitch > 107) {
                pitch = 107;
            } else if (pitch < 24) {
                pitch = 24;
            }

            // Too Short
            if (len < 15) {
                prevEnd = end;
                continue;
            }

            // Blank Interval
            if (prevEnd >= start || (prevEnd + 15 >= start)) {
                // int tick = double(prevStart) / resolution * 480;
                int prevLen = double(start - prevStart) / resolution * 480;
                notes.append(QLinkNote(prevPitch, prevLen));
            } else {
                int prevLen = double(prevEnd - prevStart) / resolution * 480;
                notes.append(QLinkNote(prevPitch, prevLen));
                int restLen = double(start - prevEnd) / resolution * 480;
                notes.append(QLinkNote(prevPitch, restLen, "R"));
            }

            prevStart = start;
            prevEnd = end;
            prevPitch = pitch;
        }
        if (prevEnd > prevStart + 15) {
            int prevLen = double(prevEnd - prevStart) / resolution * 480;
            notes.append(QLinkNote(prevPitch, prevLen));
        } else if (!notes.isEmpty()) {
            notes.back().length += prevEnd - prevStart;
        }

        // Save map
        {
            int curStart = 0;
            for (int i = 0; i < notes.size(); ++i) {
                noteMap.insert(curStart, i);
                curStart += notes.at(i).length;
            }
        }

        // Change Lyrics
        for (int i = 0; i < lyrics.size(); ++i) {
            auto it = noteMap.find(lyrics.at(i).first);
            if (it == noteMap.end()) {
                continue;
            }
            notes[it.value()].lyric = lyrics.at(i).second;
        }

        // Change Tempo
        for (int i = 0; i < tempos.size(); ++i) {
            auto it = noteMap.find(tempos.at(i).first);
            if (it == noteMap.end()) {
                continue;
            }
            notes[it.value()].tempo = tempos.at(i).second;
        }

        // Update Tempo
        if (!notes.isEmpty()) {
            if (notes[0].tempo == NODEF_DOUBLE) {
                if (tempos.isEmpty()) {
                    notes[0].tempo = DEFAULT_VALUE_TEMPO;
                } else {
                    notes[0].tempo = tempos[0].second;
                }
            }
            oNotes.setData(notes);
            oNotes.setInitTempo(notes[0].tempo);
        }
    }

    oNotes.setTrackName(trackNames[track]);
    return true;
}
