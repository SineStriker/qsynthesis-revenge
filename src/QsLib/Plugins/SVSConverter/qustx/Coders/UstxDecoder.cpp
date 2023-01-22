#include "UstxDecoder.h"
#include "Util/MusicMath.h"

#include <QtMath>

UstxDecoder::UstxDecoder() {
}

UstxDecoder::~UstxDecoder() {
}

QSvipModel UstxDecoder::DecodeProject(const UProject &ustxProject) {
    //曲速：OpenUTAU每个工程只有一个曲速
    QList<QSvipModel::SongTempo> songTempoList;
    songTempoList.append(QSvipModel::SongTempo{0, ustxProject.bpm});

    //节拍：OpenUTAU每个工程只有一个节拍
    QList<QSvipModel::TimeSignature> timeSignatureList;
    timeSignatureList.append(
        QSvipModel::TimeSignature{ustxProject.beatPerBar, ustxProject.beatUnit, 0});

    //音轨
    QList<QSvipModel::TrackRef> trackList;
    for (const UTrack &ustxTrack : qAsConst(ustxProject.tracks)) {
        auto track = new QSvipModel::SingingTrack();
        *track = DecodeTrack(ustxTrack);
        trackList.append(QSvipModel::TrackRef(track));
    }

    //区段：OpenUTAU的音轨和区段分开存储，因此这里一个个把区段塞进音轨
    for (const UVoicePart &ustxVoicePart : qAsConst(ustxProject.voiceParts)) {
        DecodeVoicePart(
            ustxVoicePart,
            *static_cast<QSvipModel::SingingTrack *>(trackList[ustxVoicePart.trackNo].data()),
            ustxProject);
    }

    QSvipModel osProject;

    osProject.Version = "SVIP7.0.0";
    osProject.SongTempoList = std::move(songTempoList);
    osProject.TimeSignatureList = std::move(timeSignatureList);
    osProject.TrackList = std::move(trackList);

    return osProject;
}

QSvipModel::SingingTrack UstxDecoder::DecodeTrack(const UTrack &ustxTrack) {
    QSvipModel::SingingTrack osTrack;

    osTrack.Title = QString();
    osTrack.Mute = ustxTrack.mute;
    osTrack.Solo = ustxTrack.solo;

    // OpenUTAU的音量以分贝存储（#TODO:待实测），这里转为绝对音量
    osTrack.Volume = qMin(qPow(10, ustxTrack.volume / 10), 2.0);
    osTrack.Pan = 0;
    osTrack.AISingerName = ustxTrack.singer;

    return osTrack;
}

void UstxDecoder::DecodeVoicePart(const UVoicePart &ustxVoicePart,
                                  QSvipModel::SingingTrack &osTrack, const UProject &ustxProject) {
    int partOffset = ustxVoicePart.position;
    for (const UNote &ustxNote : qAsConst(ustxVoicePart.notes)) {
        osTrack.NoteList.append(DecodeNote(ustxNote, partOffset));
    }
    osTrack.EditedParams.Pitch.PointList.append(DecodePitch(ustxVoicePart, ustxProject));
    if (osTrack.Title.isEmpty()) {
        osTrack.Title = ustxVoicePart.name; //音轨名称采用第一个区段的名称
    }
}

QSvipModel::Note UstxDecoder::DecodeNote(const UNote &ustxNote, int partOffset) {
    QString lyric = ustxNote.lyric;

    // OpenUTAU的连音符为+，多音节词可能还有+~、+4等形式，这里统一转为-
    if (lyric.startsWith("+")) {
        lyric = "-";
    }

    QSvipModel::Note note;
    note.StartPos = ustxNote.position + partOffset;
    note.Length = ustxNote.duration;
    note.KeyNumber = ustxNote.tone;
    note.Lyric = std::move(lyric);
    return note;
}

QList<QPair<int, int>> UstxDecoder::DecodePitch(const UVoicePart &part, const UProject &project) {
    int pitchStart;
    auto uNotes = part.notes; //音符列表
    QSharedPointer<UNote> prevNote;

    //============================================
    const int pitchInterval = 5; //每5tick一个音高点
    pitchStart = 0;              //音高线起点：0
    QVector<double> pitches;     //音高线长度。音高线终点为结尾音素的末端
    pitches.resize(qMax(part.duration(), part.notes.last().end()) / pitchInterval);

    int index = 0;
    for (const auto &note : qAsConst(uNotes)) {
        while (pitchStart + index * pitchInterval < note.end() && index < pitches.size()) {
            pitches[index] = note.tone * 100;
            index++;
        } //基础音高线为阶梯，只管当前处于哪个音符
    }
    index = qMax(1, index);
    while (index < pitches.size()) {
        pitches[index] = pitches[index - 1]; //结尾如果还有多余的地方，就用最后一个音符的音高填充
        index++;
    }

    for (const auto &note : qAsConst(uNotes)) { //对每个音符
        if (note.vibrato.length() <= 0) { //如果音符的颤音长度<=0，则无颤音。颤音长度按毫秒存储
            continue;
        }
        int startIndex = qMax(0, qCeil(double(note.position - pitchStart) /
                                       pitchInterval)); //音符起点在采样音高线上的x坐标
        int endIndex = qMin(pitches.length(), (note.end() - pitchStart) /
                                                  pitchInterval); //音符终点在采样音高线上的x坐标
        for (int i = startIndex; i < endIndex; ++i) {
            double nPos = double(pitchStart + i * pitchInterval - note.position) /
                          note.duration; //音符长度，单位为5tick
            double nPeriod = double(project.MillisecondToTick(note.vibrato.period())) /
                             note.duration; //颤音长度，单位为5tick
            auto point = note.vibrato.evaluate(
                nPos, nPeriod, note); //将音符长度颤音长度代入进去，求出带颤音的音高线
            pitches[i] = point.y() * 100;
        }
    }
    for (int i = 0; i < uNotes.size(); i++) { //对每个音符
        const auto &note = uNotes.at(i);
        QList<PitchPoint> pitchPoints;
        pitchPoints.reserve(note.pitch.data.size());
        for (const auto &point : qAsConst(note.pitch.data)) {
            pitchPoints.append(
                PitchPoint( // OpenUTAU的控制点按毫秒存储（这个设计会导致修改曲速时出现混乱），这里先转成tick
                    project.MillisecondToTick(point.X) + note.position,
                    point.Y * 10 + note.tone * 100, point.shape));
        }
        if (pitchPoints.size() == 0) { //如果没有控制点，则默认台阶形
            pitchPoints.append(PitchPoint(note.position, note.tone * 100));
            pitchPoints.append(PitchPoint(note.end(), note.tone * 100));
        }
        if (i == 0 && pitchPoints[0].X > pitchStart) {
            pitchPoints.prepend(PitchPoint(
                pitchStart,
                pitchPoints[0]
                    .Y)); //如果整个段落开头有控制点没覆盖到的地方（以音素开头为准），则向前水平延伸
        } else if (pitchPoints[0].X > note.position) {
            pitchPoints.prepend(
                PitchPoint(note.position, pitchPoints[0].Y)); //对于其他音符，则以卡拍点为准
        }
        if (pitchPoints.last().X < note.end()) {
            pitchPoints.append(PitchPoint(
                note.end(),
                pitchPoints.last().Y)); //如果整个段落结尾有控制点没覆盖到的地方，则向后水平延伸
        }
        PitchPoint lastPoint = pitchPoints[0]; //现在lastpoint是第一个控制点
        index = qMax(
            0,
            int((lastPoint.X - pitchStart) /
                pitchInterval)); //起点在采样音高线上的x坐标，以5tick为单位。如果第一个控制点在0前面，就从0开始，否则从第一个控制点开始
        for (const auto &point : pitchPoints.mid(1)) {      //对每一段曲线
            int x = pitchStart + index * pitchInterval;     //起点在工程中的x坐标
            while (x < point.X && index < pitches.size()) { //遍历采样音高点
                double pitch = MusicMath::InterpolateShape(
                    lastPoint.X, point.X, lastPoint.Y, point.Y, x,
                    (int) lastPoint.shape); //绝对音高。插值，正式将控制点转化为曲线！
                double basePitch = !prevNote.isNull() && x < prevNote->end()
                                       ? prevNote->tone * 100
                                       : note.tone * 100; //台阶基础音高
                pitches[index] += pitch - basePitch;      //锚点音高比基础音高高了多少
                index++;
                x += pitchInterval;
            }
            lastPoint = point;
        }
        prevNote.reset(new UNote(note));
    }

    UCurve curve;
    // PITD为手绘音高线差值。这里从ustx工程中尝试调取该参数
    for (const auto &c : qAsConst(part.curves)) {
        if (c.abbr == "pitd") {
            curve = c;
            break;
        }
    }
    if (!curve.isEmpty()) { //如果参数存在
        for (int i = 0; i < pitches.size(); ++i) {
            pitches[i] += curve.sample(pitchStart + i * pitchInterval); //每个点加上PITD的值
        }
    }

    //============================================
    QList<QPair<int, int>> pointList;
    int firstBarLength = 1920 * project.beatPerBar / project.beatUnit;
    pointList.append(qMakePair(firstBarLength + part.position, -100));
    for (int i = 0; i < pitches.size(); ++i) {
        pointList.append(qMakePair(firstBarLength + part.position + i * pitchInterval, pitches[i]));
    }
    pointList.append(
        qMakePair(firstBarLength + part.position + pitches.size() * pitchInterval, -100));
    return pointList;
}
