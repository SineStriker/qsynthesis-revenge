#include "UstxDecoder.h"

#include <QtMath>

UstxDecoder::UstxDecoder() {
}

UstxDecoder::~UstxDecoder() {
}

QSvipFile UstxDecoder::DecodeProject(const UProject &ustxProject) {
    //曲速：OpenUTAU每个工程只有一个曲速
    QList<QSvipFile::SongTempo> songTempoList;
    songTempoList.append(QSvipFile::SongTempo{0, ustxProject.bpm});

    //节拍：OpenUTAU每个工程只有一个节拍
    QList<QSvipFile::TimeSignature> timeSignatureList;
    timeSignatureList.append(QSvipFile::TimeSignature{
        ustxProject.beatPerBar,
        ustxProject.beatUnit,
    });

    //音轨
    QList<QSvipFile::TrackRef> trackList;
    for (const UTrack &ustxTrack : qAsConst(ustxProject.tracks)) {
        auto track = new QSvipFile::SingingTrack();
        *track = DecodeTrack(ustxTrack);
        trackList.append(QSvipFile::TrackRef(track));
    }

    //区段：OpenUTAU的音轨和区段分开存储，因此这里一个个把区段塞进音轨
    for (const UVoicePart &ustxVoicePart : qAsConst(ustxProject.voiceParts)) {
        DecodeVoicePart(
            ustxVoicePart,
            *static_cast<QSvipFile::SingingTrack *>(trackList[ustxVoicePart.trackNo].data()),
            ustxProject);
    }

    QSvipFile osProject;

    osProject.Version = "SVIP7.0.0";
    osProject.SongTempoList = std::move(songTempoList);
    osProject.TimeSignatureList = std::move(timeSignatureList);
    osProject.TrackList = std::move(trackList);

    return osProject;
}

QSvipFile::SingingTrack UstxDecoder::DecodeTrack(const UTrack &ustxTrack) {
    QSvipFile::SingingTrack osTrack;

    osTrack.Title = QString();
    osTrack.Mute = ustxTrack.mute;
    osTrack.Solo = ustxTrack.solo;

    // OpenUTAU的音量以分贝存储（#TODO:待实测），这里转为绝对音量
    osTrack.Volume = qMin(qPow(10, ustxTrack.volume / 10), 2.0);
    osTrack.Pan = 0;
    osTrack.AISingerName = ustxTrack.singer;

    return osTrack;
}

void UstxDecoder::DecodeVoicePart(const UVoicePart &ustxVoicePart, QSvipFile::SingingTrack &osTrack,
                                  const UProject &ustxProject) {
    int partOffset = ustxVoicePart.position;
    for (const UNote &ustxNote : qAsConst(ustxVoicePart.notes)) {
        osTrack.NoteList.append(DecodeNote(ustxNote, partOffset));
    }
    osTrack.EditedParams.Pitch.PointList.append(DecodePitch(ustxVoicePart, ustxProject));
    if (osTrack.Title.isEmpty()) {
        osTrack.Title = ustxVoicePart.name; //音轨名称采用第一个区段的名称
    }
}

QSvipFile::Note UstxDecoder::DecodeNote(const UNote &ustxNote, int partOffset) {
    QString lyric = ustxNote.lyric;

    // OpenUTAU的连音符为+，多音节词可能还有+~、+4等形式，这里统一转为-
    if (lyric.startsWith("+")) {
        lyric = "-";
    }

    QSvipFile::Note note;
    note.StartPos = ustxNote.position + partOffset;
    note.Length = ustxNote.duration;
    note.KeyNumber = ustxNote.tone;
    note.Lyric = std::move(lyric);

    return note;
}

QList<QPair<int, int>> UstxDecoder::DecodePitch(const UVoicePart &part, const UProject &project) {
    return {};
}
