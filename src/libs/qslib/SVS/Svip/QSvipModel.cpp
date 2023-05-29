#include "QSvipModel.h"

#include <QJsonArray>

QSvipModel::QSvipModel() {
}

QSvipModel::~QSvipModel() {
}

QJsonObject QSvipModel::toJsonObject() const {
    auto encodeParam = [](const ParamCurve &param) -> QJsonObject {
        QJsonObject obj;

        obj.insert("TotalPointsCount", param.TotalPointsCount());
        QJsonArray arr;
        for (const auto &item : qAsConst(param.PointList)) {
            arr.append(QJsonArray({item.first, item.second}));
        }
        obj.insert("PointList", arr);

        return obj;
    };

    QJsonObject objDoc;
    objDoc.insert("Version", Version);

    // TempoList
    {
        QJsonArray arr;
        for (const auto &item : qAsConst(SongTempoList)) {
            arr.append(QJsonObject({{"Position", item.Position}, {"BPM", item.BPM}}));
        }
        objDoc.insert("SongTempoList", arr);
    }

    // BeatList
    {
        QJsonArray arr;
        for (const auto &item : qAsConst(TimeSignatureList)) {
            arr.append(QJsonObject({{"BarIndex", item.BarIndex},
                                    {"Denominator", item.Denominator},
                                    {"Numerator", item.Numerator}}));
        }
        objDoc.insert("TimeSignatureList", arr);
    }

    // TrackList
    {
        QJsonArray arr;
        for (const auto &item : qAsConst(TrackList)) {
            QJsonObject objTrack;
            objTrack.insert("Title", item->Title);
            objTrack.insert("Mute", item->Mute);
            objTrack.insert("Solo", item->Solo);
            objTrack.insert("Volume", item->Volume);
            objTrack.insert("Pan", item->Pan);

            if (item->type() == Track::Singing) {
                auto track = dynamic_cast<const SingingTrack *>(item.data());

                objTrack.insert("Type", "Singing");
                objTrack.insert("AISingerName", track->AISingerName);
                objTrack.insert("ReverbPreset", track->ReverbPreset);

                QJsonArray noteArr;
                for (const auto &note : qAsConst(track->NoteList)) {
                    QJsonObject objNote;
                    objNote.insert("StartPos", note.StartPos);
                    objNote.insert("Length", note.Length);
                    objNote.insert("KeyNumber", note.KeyNumber);
                    objNote.insert("HeadTag", note.HeadTag);
                    objNote.insert("Lyric", note.Lyric);
                    objNote.insert("Pronunciation", note.Pronunciation);
                    objNote.insert(
                        "EditedPhones",
                        note.EditedPhones.isNull()
                            ? QJsonValue(QJsonValue::Null)
                            : QJsonObject(
                                  {{"HeadLengthInSecs", note.EditedPhones->HeadLengthInSecs},
                                   {"MidRatioOverTail", note.EditedPhones->MidRatioOverTail}}));
                    objNote.insert(
                        "Vibrato",
                        note.Vibrato.isNull()
                            ? QJsonValue(QJsonValue::Null)
                            : QJsonObject({{"StartPercent", note.Vibrato->StartPercent},
                                           {"StartPercent", note.Vibrato->StartPercent},
                                           {"EndPercent", note.Vibrato->EndPercent},
                                           {"Amplitude", encodeParam(note.Vibrato->Amplitude)},
                                           {"Frequency", encodeParam(note.Vibrato->Frequency)}}));
                    noteArr.append(objNote);
                }
                objTrack.insert("NoteList", noteArr);

                objTrack.insert("EditedParams",
                                QJsonObject({
                                    {"Pitch", encodeParam(track->EditedParams.Pitch)},
                                    {"Volume", encodeParam(track->EditedParams.Volume)},
                                    {"Breath", encodeParam(track->EditedParams.Breath)},
                                    {"Gender", encodeParam(track->EditedParams.Gender)},
                                    {"Strength", encodeParam(track->EditedParams.Strength)},
                                }));
            } else {
                auto track = dynamic_cast<const InstrumentalTrack *>(item.data());
                objTrack.insert("Type", "Instrumental");
                objTrack.insert("AudioFilePath", track->AudioFilePath);
                objTrack.insert("Offset", track->Offset);
            }
            arr.append(objTrack);
        }
        objDoc.insert("TrackList", arr);
    }

    return objDoc;
}

QSvipModel QSvipModel::fromJsonObject(const QJsonObject &obj, bool *ok) {
    auto decodeParam = [](const QJsonObject &obj) -> ParamCurve {
        auto arrPoints = obj.value("PointList").toArray();
        ParamCurve curve;
        for (int i = 0; i < arrPoints.count(); i++) {
            auto objPoint = arrPoints.at(i).toArray();
            QPair<int, int> point = {
                objPoint.at(0).toInt(),
                objPoint.at(1).toInt()
            };
            curve.PointList.append(point);
        }
        return curve;
    };

    QSvipModel model;

    // Version
    model.Version = obj.value("Version").toString();
    qDebug() << model.Version;

    // TempoList
    auto arrTempos = obj.value("SongTempoList").toArray();
    for (int i = 0; i < arrTempos.count(); i++) {
        auto objTempo = arrTempos.at(i).toObject();
        QSvipModel::SongTempo tempo;
        tempo.Position = objTempo.value("Position").toInt();
        tempo.BPM = objTempo.value("BPM").toDouble();
        model.SongTempoList.append(tempo);
        qDebug() << model.SongTempoList.at(0).BPM;
    }

    // TimeSignatureList
    auto arrTimeSignatures = obj.value("TimeSignatureList").toArray();
    for (int i = 0; i < arrTimeSignatures.count(); i++) {
        auto objTimeSig = arrTimeSignatures.at(i).toObject();
        QSvipModel::TimeSignature timeSig;
        timeSig.BarIndex = objTimeSig.value("BarIndex").toInt();
        timeSig.Numerator = objTimeSig.value("Numerator").toInt();
        timeSig.Denominator = objTimeSig.value("Denominator").toInt();
        model.TimeSignatureList.append(timeSig);
        qDebug() << model.TimeSignatureList.at(0).Numerator;
        qDebug() << model.TimeSignatureList.at(0).Denominator;
    }

    // TrackList
    auto arrTracks = obj.value("TrackList").toArray();
    for (int i = 0; i < arrTracks.count(); i++) {
        auto objTrack = arrTracks.at(i).toObject();
        auto type = objTrack.value("Type").toString();
        if (type == "Singing") {
            auto* singingTrackPtr = new QSvipModel::SingingTrack;
            QSharedPointer<QSvipModel::SingingTrack> singingTrack(singingTrackPtr);
            singingTrack->Title = objTrack.value("Title").toString();
            singingTrack->Mute = objTrack.value("Mute").toBool();
            singingTrack->Solo = objTrack.value("Solo").toBool();
            singingTrack->Volume = objTrack.value("Volume").toDouble();
            singingTrack->Pan = objTrack.value("Pan").toDouble();

            singingTrack->AISingerName = objTrack.value("AISingerName").toString();
            singingTrack->ReverbPreset = objTrack.value("ReverbPreset").toString();

            auto arrNotes = objTrack.value("NoteList").toArray();
            for (int i = 0; i < arrNotes.count(); i++) {
                auto objNote = arrNotes.at(i).toObject();
                QSvipModel::Note note;
                note.StartPos = objNote.value("StartPos").toInt();
                note.Length = objNote.value("Length").toInt();
                note.KeyNumber = objNote.value("KeyNumber").toInt();
                note.HeadTag = objNote.value("HeadTag").toString();
                note.Lyric = objNote.value("Lyric").toString();
                note.Pronunciation = objNote.value("Pronunciation").toString();

                auto objPhones = objNote.value("EditedPhones").toObject();
                auto* phonesPtr = new QSvipModel::Phones;
                QSharedPointer<QSvipModel::Phones> phones(phonesPtr);
                phones->HeadLengthInSecs = objPhones.value("HeadLengthInSecs").toDouble();
                phones->MidRatioOverTail = objPhones.value("MidRatioOverTail").toDouble();
                note.EditedPhones = phones;
                // Vibrato was obsoleted
                singingTrack->NoteList.append(note);
            }

            auto objParams = objTrack.value("Params").toObject();
            QSvipModel::Params params;
            params.Pitch = decodeParam(objParams.value("Pitch").toObject());
            params.Volume = decodeParam(objParams.value("Volume").toObject());
            params.Breath = decodeParam(objParams.value("Breath").toObject());
            params.Gender = decodeParam(objParams.value("Gender").toObject());
            params.Strength = decodeParam(objParams.value("Strength").toObject());
            singingTrack->EditedParams = params;
            model.TrackList.append(singingTrack);
        } else if (type == "Instrumental") {
            auto* instTrackPtr = new QSvipModel::InstrumentalTrack;
            QSharedPointer<QSvipModel::InstrumentalTrack> instTrack(instTrackPtr);
            instTrack->Title = objTrack.value("Title").toString();
            instTrack->Mute = objTrack.value("Mute").toBool();
            instTrack->Solo = objTrack.value("Solo").toBool();
            instTrack->Volume = objTrack.value("Volume").toDouble();
            instTrack->Pan = objTrack.value("Pan").toDouble();

            instTrack->AudioFilePath = objTrack.value("AudioFilePath").toString();
            instTrack->Offset = objTrack.value("Offset").toDouble();
            model.TrackList.append(instTrack);
        }
    }
    qDebug() << model.TrackList.count();

    return model;
}

void QSvipModel::reset() {
    Version.clear();
    SongTempoList.clear();
    TimeSignatureList.clear();
    TrackList.clear();
}
