#ifndef UTAPROJECTTEXT_H
#define UTAPROJECTTEXT_H

#include <QString>

namespace Utau {

    // Sections
    const char SECTION_BEGIN_MARK[] = "[#";
    const char SECTION_END_MARK[] = "]";
    const char SECTION_NAME_DELETE[] = "DELETE";
    const char SECTION_NAME_INSERT[] = "INSERT";
    const char SECTION_NAME_NEXT[] = "NEXT";
    const char SECTION_NAME_PREV[] = "PREV";
    const char SECTION_NAME_SETTING[] = "SETTING";
    const char SECTION_NAME_VERSION[] = "VERSION";
    const char SECTION_NAME_TRACKEND[] = "TRACKEND"; // Add

    // Keys
    const char KEY_NAME_CHARSET[] = "Charset"; // Add
    const char KEY_NAME_PROJECT[] = "Project"; // Add
    const char KEY_NAME_CACHE_DIR[] = "CacheDir";
    const char KEY_NAME_DIRECT[] = "$direct";
    const char KEY_NAME_ENVELOPE[] = "Envelope";
    const char KEY_NAME_FLAGS[] = "Flags";
    const char KEY_NAME_INTENSITY[] = "Intensity";
    const char KEY_NAME_LENGTH[] = "Length";
    const char KEY_NAME_LYRIC[] = "Lyric";
    const char KEY_NAME_MODULATION[] = "Modulation";
    const char KEY_NAME_MODURATION[] = "Moduration";
    const char KEY_NAME_NOTE_NUM[] = "NoteNum";
    const char KEY_NAME_PBM[] = "PBM";
    const char KEY_NAME_PBS[] = "PBS";
    const char KEY_NAME_PB_TYPE[] = "PBType";
    const char KEY_NAME_PB_START[] = "PBStart"; // Add
    const char KEY_NAME_PBW[] = "PBW";
    const char KEY_NAME_PBY[] = "PBY";
    const char KEY_NAME_PICHES[] = "Piches";
    const char KEY_NAME_PITCHES[] = "Pitches";
    const char KEY_NAME_PITCH_BEND[] = "PitchBend"; // Add
    const char KEY_NAME_PRE_UTTERANCE[] = "PreUtterance";
    const char KEY_NAME_START_POINT[] = "StartPoint";
    const char KEY_NAME_TEMPO[] = "Tempo";
    const char KEY_NAME_VBC[] = "VBC";
    const char KEY_NAME_VBR[] = "VBR";
    const char KEY_NAME_VELOCITY[] = "Velocity";
    const char KEY_NAME_VOICE_DIR[] = "VoiceDir";
    const char KEY_NAME_VOICE_OVERLAP[] = "VoiceOverlap";

    // Other Keys
    const char KEY_NAME_PROJECT_NAME[] = "ProjectName"; // Add
    const char KEY_NAME_TRACKS[] = "Tracks";            // Add
    const char KEY_NAME_OUTPUT_FILE[] = "OutFile";      // Add
    const char KEY_NAME_MODE2[] = "Mode2";              // Add
    const char KEY_NAME_TOOL1[] = "Tool1";              // Add
    const char KEY_NAME_TOOL2[] = "Tool2";              // Add

    const char KEY_NAME_PRE_UTTERANCE_READONLY[] = "@preuttr";
    const char KEY_NAME_VOICE_OVERLAP_READONLY[] = "@overlap";
    const char KEY_NAME_START_POINT_READONLY[] = "@stpoint";

    const char KEY_NAME_FILENAME_READONLY[] = "@filename";
    const char KEY_NAME_ALIAS_READONLY[] = "@alias";
    const char KEY_NAME_CACHE_READONLY[] = "@cache";

    const char KEY_NAME_PATCH[] = "$patch";
    const char KEY_NAME_REGION_START[] = "$region";
    const char KEY_NAME_REGION_END[] = "$region_end";

    const char KEY_NAME_LABEL[] = "Label";

    // Head
    const char UST_VERSION_PREFIX[] = "UST Version ";
    const char UST_VERSION_PREFIX_NOSPACE[] = "UST Version";

}

#endif // UTAPROJECTTEXT_H
