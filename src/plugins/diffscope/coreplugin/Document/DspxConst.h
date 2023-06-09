#ifndef CHORUSKIT_DSPXCONST_H
#define CHORUSKIT_DSPXCONST_H

namespace Core::Dspx {

    // Metadata
    const char Version[] = "1.0.0";
    const char DefaultAuthor[] = "";
    const char DefaultProjectName[] = "New Project";

    // Control
    const double DefaultGain = 0;
    const double DefaultPan = 0;
    const bool DefaultMute = false;
    const bool DefaultSolo = false;

    // Timeline
    const int DefaultTimeSignaturePos = 0;
    const int DefaultTimeSignatureNumerator = 4;
    const int DefaultTimeSignatureDenominator = 4;

    // Tempo
    const int DefaultTempoPos = 0;
    const double DefaultTempoValue = 120.0;

    // Label
    const int DefaultLabelPos = 0;
    const char DefaultLabelText[] = "";

    // Track
    const char DefaultTrackName[] = "Untitled Track";

    // Clip
    const char DefaultSingingClipName[] = "Untitled Clip";
    const char DefaultAudioClipName[] = "Untitled Clip";
    const char DefaultAudioClipPath[] = "/path/to";
    const int DefaultClipStart = 0;
    const int DefaultClipLength = 480;
    const int DefaultClipClipStart = 0;
    const int DefaultClipClipLength = 480;

    // Note
    const int DefaultNotePos = 0;
    const int DefaultNoteLength = 480;
    const int DefaultNoteKeyNum = 60;
    const char DefaultNoteLyric[] = "la";

    // Vibrato
    const double DefaultVibratoStart = 0;
    const double DefaultVibratoEnd = 1;
    const double DefaultVibratoFrequency = 5;
    const double DefaultVibratoPhase = 0;
    const double DefaultVibratoAmplitude = 1;
    const double DefaultVibratoOffset = 0;
    const double DefaultVibratoPointX = 0;
    const double DefaultVibratoPointY = 0;

    // Phoneme
    const char DefaultPhonemeType[] = "ahead";
    const char DefaultPhonemeToken[] = "";
    const double DefaultPhonemeStart = 0;

    // ParamAnchor
    const char DefaultParamAnchorNodeInterpolation[] = "linear";
    const int DefaultParamAnchorNodeX = 0;
    const int DefaultParamAnchorNodeY = 0;

    // ParamFree
    const int DefaultParamFreeStart = 0;
    const int DefaultParamFreeStep = 5;

}

#endif // CHORUSKIT_DSPXCONST_H
