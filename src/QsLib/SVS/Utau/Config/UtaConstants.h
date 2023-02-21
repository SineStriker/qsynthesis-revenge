#ifndef UTACONSTANTS_H
#define UTACONSTANTS_H

namespace Utau {

    // Constants
    const char TONE_NAMES[] = "CCDDEFFGGAAB";
    const char TONE_NAME_SHARP[] = "#";
    const char LYRIC_R[] = "R";

    const int TIME_BASE = 480;
    const int TONE_NUMBER_BASE = 24;
    const int TONE_OCTAVE_MAX = 7;
    const int TONE_OCTAVE_MIN = 1;
    const int TONE_OCTAVE_STEPS = 12;

    // Default
    const double DEFAULT_VALUE_INTENSITY = 100.0;
    const double DEFAULT_VALUE_MODULATION = 100.0; // Add
    const double DEFAULT_VALUE_PRE_UTTERANCE = 0.0;
    const double DEFAULT_VALUE_TEMPO = 120.0;
    const double DEFAULT_VALUE_VOICE_OVERLAP = 0.0;

    const double DEFAULT_VALUE_VELOCITY = 100.0;
    const double DEFAULT_VALUE_START_POINT = 0.0;
    const char DEFAULT_VALUE_FLAGS[] = "";

    // Values
    const char VALUE_MODE2_ON[] = "True";
    const char VALUE_TRACKS_SINGLE[] = "1";
    const char VALUE_PITCH_TYPE[] = "5";

    const char UST_VERSION_1_2[] = "1.2";

    // Escape
    const char DIR_VOICE_BASE[] = "%VOICE%";

};

#endif // UTACONSTANTS_H
