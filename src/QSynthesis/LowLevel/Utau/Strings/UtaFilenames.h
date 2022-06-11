#ifndef UTAFILENAMES_H
#define UTAFILENAMES_H

#include <QString>

namespace Utau {

    const char FILE_NAME_PLUGIN_TEXT[] = "plugin.txt";
    const char FILE_NAME_CHAR_TEXT[] = "character.txt";

    const char FILE_NAME_VOICE_README[] = "readme.txt";
    const char FILE_NAME_OTO_INI[] = "oto.ini";
    const char FILE_NAME_PREFIX_MAP[] = "prefix.map";

    const char FILE_NAME_TEMP_AUDIO[] = "temp.wav";

    const char FILE_NAME_SETTING_INI[] = "setting.ini";

// Cross-platform File Names
#ifdef Q_OS_WINDOWS
    const char FILE_NAME_TEMP_BATCH[] = "temp.bat";
    const char FILE_NAME_HELPER_BATCH[] = "temp_helper.bat";
#elif defined(Q_OS_LINUX) || defined(Q_OS_MAC)
    const char FILE_NAME_TEMP_BATCH[] = "temp.sh";
    const char FILE_NAME_HELPER_BATCH[] = "temp_helper.sh";
#endif

    const char DIR_NAME_VOICE[] = "voice";
    const char DIR_NAME_PLUGIN[] = "plugins";

} // namespace Utau

#endif // UTAFILENAMES_H
