#ifndef USTXENCODER_H
#define USTXENCODER_H

#include "Svip/QSvipFile.h"
#include "Ustx/UProject.h"

class UstxEncoder {
public:
    UstxEncoder();
    ~UstxEncoder();

public:
    UProject EncodeProject(const QSvipFile &osProject);
    UTrack EncodeTrack(const QSvipFile::Track &osTrack);
    UVoicePart EncodeVoicePart(const QSvipFile::SingingTrack &osTrack, int trackNo);
    UWavePart EncodeWavePart(const QSvipFile::InstrumentalTrack &osTrack, int trackNo);
    UNote EncodeNote(const QSvipFile::Note &osNote, bool snapFirst, int lastNoteKeyNumber);
};

#endif // USTXENCODER_H
