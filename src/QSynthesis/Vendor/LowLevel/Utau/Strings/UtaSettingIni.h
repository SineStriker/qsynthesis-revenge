#ifndef UTASETTINGINI_H
#define UTASETTINGINI_H

#include <QString>

// setting.ini is no longer used in subsequent commits
namespace Utau {

    const char KEY_NAME_CACHE_ENABLE[] = "CacheEnabled";
    const char KEY_NAME_CACHE_DELETE_BY_SIZE[] = "CacheDeleteBySize";
    const char KEY_NAME_CACHE_DELETE_SIZE[] = "CacheDeleteSize";
    const char KEY_NAME_NO_OTO_REPORT[] = "NoOtoReport";
    const char KEY_NAME_NO_COPY_GENON[] = "NoCopyVoiceParam";
    const char KEY_NAME_NO_BATCH_MODE[] = "NoBatchMode";
    const char KEY_NAME_PROCESSES[] = "Processes";
    const char KEY_NAME_TOOL1_PATH[] = "Tool1Path";
    const char KEY_NAME_TOOL2_PATH[] = "Tool2Path";

    const char KEY_PREFIX_PROJECT[] = "History";

    const char KEY_NAME_MAINWINDOW[] = "MainWindow";

} // namespace Utau

#endif // UTASETTINGINI_H
