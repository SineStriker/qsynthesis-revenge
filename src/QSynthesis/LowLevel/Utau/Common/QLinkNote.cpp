#include "QLinkNote.h"
#include "QControlPoint.h"

using namespace Utau;

Note::Note() {
    clear();
}

Note::Note(int noteNum, int length) {
    clear();
    this->noteNum = noteNum;
    this->length = length;
}

Note::Note(int noteNum, int length, const QString &lrc) {
    clear();
    this->noteNum = noteNum;
    this->length = length;
    this->lyric = lrc;
}

void Note::clear() {
    lyric = "a";
    flags = "";

    noteNum = StandardToneNum();
    length = TIME_BASE;

    velocity = preUttr = overlap = stp = NODEF_DOUBLE;
    intensity = 100.0;
    modulation = 0.0;

    tempo = NODEF_DOUBLE;

    envelope = {};

    Mode2Pitch = {};
    vibrato = {};

    pbstart = NODEF_DOUBLE;
    pitches = {};
    pbtype = "";

    label = direct = patch = region = regionEnd = "";

    customData = {};
}

QJsonObject Note::toJson() const {
    QJsonObject obj;

    obj.insert("Lyric", lyric);
    obj.insert("Length", length);
    obj.insert("NoteNum", noteNum);
    obj.insert("Flags", flags);

    obj.insert("Intensity", intensity);
    obj.insert("Modulation", modulation);
    obj.insert("PreUtterance", preUttr);

    obj.insert("VoiceOverlap", overlap);
    obj.insert("Velocity", velocity);
    obj.insert("StartPoint", stp);

    obj.insert("Tempo", tempo);

    obj.insert("PBType", pbtype);
    obj.insert("PBStart", pbstart);

    if (!pitches.isEmpty()) {
        QJsonArray arr;
        for (int i = 0; i < pitches.size(); ++i) {
            arr.append(vibrato.at(i));
        }
        obj.insert("PitchBend", arr);
    }

    if (!vibrato.isEmpty()) {
        QJsonArray arr;
        for (int i = 0; i < vibrato.size(); ++i) {
            arr.append(vibrato.at(i));
        }
        obj.insert("Vibrato", arr);
    }

    if (!envelope.isEmpty()) {
        QJsonArray arr;
        for (int i = 0; i < envelope.size(); ++i) {
            QJsonObject pt;
            pt.insert("X", envelope.at(i).mX);
            pt.insert("Y", envelope.at(i).mY);
            arr.append(pt);
        }
        obj.insert("Envelope", arr);
    }

    if (!Mode2Pitch.isEmpty()) {
        QJsonArray arr;
        for (int i = 0; i < Mode2Pitch.size(); ++i) {
            QJsonObject pt;
            pt.insert("X", Mode2Pitch.at(i).mX);
            pt.insert("Y", Mode2Pitch.at(i).mY);
            pt.insert("P", PointToString(Mode2Pitch.at(i).mP));
            arr.append(pt);
        }
        obj.insert("PBMode2", arr);
    }

    obj.insert("Label", label);
    obj.insert("$direct", direct);
    obj.insert("$patch", patch);
    obj.insert("$region", region);
    obj.insert("$region_end", regionEnd);

    if (!customData.isEmpty()) {
        for (int i = 0; i < customData.size(); ++i) {
            obj.insert(customData[i].first, customData[i].second);
        }
    }

    return obj;
}

Note Note::fromJson(const QJsonObject &obj) {
    QLinkNote note;

    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString key = it.key();
        QJsonValue val = it.value();
        if (key == "Lyric") {
            note.lyric = val.toString();
        } else if (key == "Length") {
            note.length = val.toInt();
        } else if (key == "NoteNum") {
            note.noteNum = val.toInt();
        } else if (key == "Flags") {
            note.flags = val.toString();
        } else if (key == "Intensity") {
            note.intensity = val.toDouble();
        } else if (key == "Modulation") {
            note.modulation = val.toDouble();
        } else if (key == "PreUtterance") {
            note.preUttr = val.toDouble();
        } else if (key == "VoiceOverlap") {
            note.overlap = val.toDouble();
        } else if (key == "Velocity") {
            note.velocity = val.toDouble();
        } else if (key == "StartPoint") {
            note.stp = val.toDouble();
        } else if (key == "Tempo") {
            note.tempo = val.toDouble();
        } else if (key == "PBType") {
            note.pbtype = val.toString();
        } else if (key == "PBStart") {
            note.pbstart = val.toDouble();
        } else if (key == "PitchBend") {
            if (val.isArray()) {
                QJsonArray arr = val.toArray();
                for (auto it = arr.begin(); it != arr.end(); ++it) {
                    note.pitches.append(it->toDouble());
                }
            }
        } else if (key == "PBMode2") {
            if (val.isArray()) {
                QJsonArray arr = val.toArray();
                for (auto it = arr.begin(); it != arr.end(); ++it) {
                    if (it->isObject()) {
                        QJsonObject pt = it->toObject();
                        QControlPoint p;
                        for (auto it2 = pt.begin(); it2 != pt.end(); ++it2) {
                            if (it2.key() == "X") {
                                p.mX = it2.value().toDouble();
                            } else if (it2.key() == "Y") {
                                p.mY = it2.value().toDouble();
                            } else if (it2.key() == "P") {
                                p.mP = StringToPoint(it2.value().toString());
                            }
                        }
                        note.Mode2Pitch.append(p);
                    }
                }
            }
        } else if (key == "Vibrato") {
            if (val.isArray()) {
                if (val.isArray()) {
                    QJsonArray arr = val.toArray();
                    for (auto it = arr.begin(); it != arr.end(); ++it) {
                        note.vibrato.append(it->toDouble());
                    }
                }
            }
        } else if (key == "Envelope") {
            if (val.isArray()) {
                QJsonArray arr = val.toArray();
                for (auto it = arr.begin(); it != arr.end(); ++it) {
                    if (it->isObject()) {
                        QJsonObject pt = it->toObject();
                        QControlPoint p;
                        for (auto it2 = pt.begin(); it2 != pt.end(); ++it2) {
                            if (it2.key() == "X") {
                                p.mX = it2.value().toDouble();
                            } else if (it2.key() == "Y") {
                                p.mY = it2.value().toDouble();
                            }
                        }
                        note.envelope.append(p);
                    }
                }
            }
        } else if (key == "Label") {
            note.label = val.toString();
        } else if (key == "$direct") {
            note.direct = val.toString();
        } else if (key == "$patch") {
            note.patch = val.toString();
        } else if (key == "$region") {
            note.region = val.toString();
        } else if (key == "$region_end") {
            note.regionEnd = val.toString();
        } else {
            note.customData.append(qMakePair(key, val.toString()));
        }
    }

    return note;
}

bool Note::operator==(const Note &note) const {
    return (length == note.length);
}

QDebug Utau::operator<<(QDebug debug, const Note &oNote) {
    QString info = QString("Note(lyric:%1; length:%2; noteNum:%3)")
                       .arg(oNote.lyric)
                       .arg(oNote.length)
                       .arg(oNote.noteNum);
    debug.noquote() << info;
    return debug;
}
