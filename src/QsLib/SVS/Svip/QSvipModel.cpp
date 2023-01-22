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

QSvipModel QSvipModel::fromJsonObject(const QJsonObject &obj) {
    Q_UNUSED(obj);
    return {};
}

void QSvipModel::reset() {
    Version.clear();
    SongTempoList.clear();
    TimeSignatureList.clear();
    TrackList.clear();
}
