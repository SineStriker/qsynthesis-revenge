#ifndef USTXENCODER_H
#define USTXENCODER_H

#include "Svip/QSvipModel.h"
#include "Ustx/UProject.h"

/**
 *  Interpret from OxygenDioxide.UstxPlugin.Stream
 */

class UstxEncoder {
public:
    UstxEncoder();
    ~UstxEncoder();

public:
    UProject EncodeProject(const QSvipModel &osProject);
    UTrack EncodeTrack(const QSvipModel::Track &osTrack);
    UVoicePart EncodeVoicePart(const QSvipModel::SingingTrack &osTrack, int trackNo);
    UWavePart EncodeWavePart(const QSvipModel::InstrumentalTrack &osTrack, int trackNo);
    UNote EncodeNote(const QSvipModel::Note &osNote, bool snapFirst, int lastNoteKeyNumber);
};

#endif // USTXENCODER_H
