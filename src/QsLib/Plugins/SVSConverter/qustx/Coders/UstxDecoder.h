#ifndef USTXDECODER_H
#define USTXDECODER_H

#include "Svip/QSvipModel.h"
#include "Ustx/UProject.h"

/**
 *  Interpret from OxygenDioxide.UstxPlugin.Stream
 */

class UstxDecoder {
public:
    UstxDecoder();
    ~UstxDecoder();

public:
    QSvipModel DecodeProject(const UProject &ustxProject);
    QSvipModel::SingingTrack DecodeTrack(const UTrack &ustxTrack);
    void DecodeVoicePart(const UVoicePart &ustxVoicePart, QSvipModel::SingingTrack &osTrack,
                         const UProject &ustxProject);
    QSvipModel::Note DecodeNote(const UNote &ustxNote, int partOffset);
    QList<QPair<int, int>> DecodePitch(const UVoicePart &part, const UProject &project);
};

#endif // USTXDECODER_H
