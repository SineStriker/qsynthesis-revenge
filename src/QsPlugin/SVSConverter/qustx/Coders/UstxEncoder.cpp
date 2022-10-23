#include "UstxEncoder.h"

#include <QtMath>

UstxEncoder::UstxEncoder() {
}

UstxEncoder::~UstxEncoder() {
}

UProject UstxEncoder::EncodeProject(const QSvipFile &osProject) {
    UProject ustxProject;

    // OpenUTAU不支持变速，因此这里采用音轨开头的bpm
    ustxProject.bpm = osProject.SongTempoList.front().BPM;
    ustxProject.beatPerBar = osProject.TimeSignatureList[0].Numerator;
    ustxProject.beatUnit = osProject.TimeSignatureList[0].Denominator;
    ustxProject.ustxVersion = "0.5";

    int trackNo = 0;
    for (const auto &osTrack : qAsConst(osProject.TrackList)) {
        ustxProject.tracks.append(EncodeTrack(*osTrack.data()));
        if (osTrack->type() == QSvipFile::Track::Singing) //合成音轨
        {
            ustxProject.voiceParts.append(
                EncodeVoicePart(*static_cast<QSvipFile::SingingTrack *>(osTrack.data()), trackNo));
        } else //伴奏音轨
        {
            ustxProject.waveParts.append(EncodeWavePart(
                *static_cast<QSvipFile::InstrumentalTrack *>(osTrack.data()), trackNo));
        }
        trackNo += 1;
    }
    return ustxProject;
}

UTrack UstxEncoder::EncodeTrack(const QSvipFile::Track &osTrack) {

    UTrack ustxTrack;

    ustxTrack.singer = QString();
    ustxTrack.phonemizer = "OpenUtau.Core.DefaultPhonemizer"; //默认音素器
    ustxTrack.renderer = "CLASSIC";                           //经典渲染器
    ustxTrack.mute = osTrack.Mute;
    ustxTrack.solo = osTrack.Solo;
    ustxTrack.volume = std::log10(osTrack.Volume) * 10; //绝对音量转对数音量

    return ustxTrack;
}

UVoicePart UstxEncoder::EncodeVoicePart(const QSvipFile::SingingTrack &osTrack, int trackNo) {
    UVoicePart ustxVoicePart;

    ustxVoicePart.name = osTrack.Title;
    ustxVoicePart.trackNo = trackNo;
    ustxVoicePart.position = 0;

    int lastNoteEndPos = -480;  //上一个音符的结束时间
    int lastNoteKeyNumber = 60; //上一个音符的音高

    for (const QSvipFile::Note &osNote : qAsConst(osTrack.NoteList)) {
        ustxVoicePart.notes.append(
            EncodeNote(osNote, lastNoteEndPos >= osNote.StartPos, lastNoteKeyNumber));
        lastNoteEndPos = osNote.StartPos + osNote.Length;
        lastNoteKeyNumber = osNote.KeyNumber;
    }

    return ustxVoicePart;
}

UWavePart UstxEncoder::EncodeWavePart(const QSvipFile::InstrumentalTrack &osTrack, int trackNo) {

    UWavePart ustxWavePart;

    ustxWavePart.name = osTrack.Title;
    ustxWavePart.trackNo = trackNo;
    ustxWavePart.position = 0;
    ustxWavePart.setFilePath(osTrack.AudioFilePath);

    return ustxWavePart;
}

UNote UstxEncoder::EncodeNote(const QSvipFile::Note &osNote, bool snapFirst,
                              int lastNoteKeyNumber) {
    // snapFirst：是否与上一个音符挨着，挨着就是True
    // lastNoteKeyNumber：上一个音符的音高
    int Y0 = 0;
    if (snapFirst == true) {
        Y0 = (lastNoteKeyNumber - osNote.KeyNumber) * 10;
    }

    QString lyric = osNote.Lyric;
    if ("-" == lyric) {
        lyric = "+";
    }

    UPitch pitch;
    pitch.snapFirst = snapFirst;
    pitch.data = {
        PitchPoint(-40, 0, PitchPoint::Shape::io),
        PitchPoint(0, 0, PitchPoint::Shape::io),
    };

    UVibrato vibrato;
    vibrato.setLength(0);
    vibrato.setPeriod(175);
    vibrato.setDepth(25);
    vibrato.setIn(10);
    vibrato.setOut(0);
    vibrato.setShift(0);
    vibrato.setDrift(0);

    UNote ustxNote;
    ustxNote.position = osNote.StartPos;
    ustxNote.duration = osNote.Length;
    ustxNote.tone = osNote.KeyNumber;
    ustxNote.lyric = lyric;
    ustxNote.pitch = pitch;
    ustxNote.vibrato = vibrato;

    Q_UNUSED(Y0)

    return ustxNote;
}
