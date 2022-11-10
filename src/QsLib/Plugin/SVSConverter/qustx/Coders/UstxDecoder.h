#ifndef USTXDECODER_H
#define USTXDECODER_H

#include "Svip/QSvipFile.h"
#include "Ustx/UProject.h"

/**
 *  Interpret from OxygenDioxide.UstxPlugin.Stream
 */

class UstxDecoder {
public:
    UstxDecoder();
    ~UstxDecoder();

public:
    QSvipFile DecodeProject(const UProject &ustxProject);
    QSvipFile::SingingTrack DecodeTrack(const UTrack &ustxTrack);
    void DecodeVoicePart(const UVoicePart &ustxVoicePart, QSvipFile::SingingTrack &osTrack,
                         const UProject &ustxProject);
    QSvipFile::Note DecodeNote(const UNote &ustxNote, int partOffset);
    QList<QPair<int, int>> DecodePitch(const UVoicePart &part, const UProject &project);
};

#endif // USTXDECODER_H
