#ifndef NAMESPACE_H
#define NAMESPACE_H

#include <QRect>
#include <QString>

#define QS_TEST
#define QS_NO_TAB_FOCUS
// #define QS_USE_APPLE_FONT


#define QS_REMOVE_TAB_FOCUS(W)                                                                     \
    W->setFocusPolicy(static_cast<Qt::FocusPolicy>(W->focusPolicy() & ~Qt::TabFocus & ~(0x8)));

namespace Qs {

    // App info
    extern const char Version[];
    extern const char AppName[];

    // Inner wavetool arguments
    extern const int SampleRate;
    extern const int SampleBits;
    extern const int Channels;

    // Font size
    extern const int FontSizeN;
    extern const int FontSizeU;

    // No defined
    const QRect NO_RECT(0, 0, 0, 0);
    const QRectF NO_RECTF(0, 0, 0, 0);

    // Modules
    const char MIME_CLIPBOARD_NOTE[] = "application/qsynthesis/notes";

    const char FILE_HEAD_DUMP[] = "qsdump";

    const char DIR_NAME_TEMP[] = "QSynthTemp";
    const char DIR_NAME_TEMP_PREFIX[] = "qs";
    const char KEY_NAME_MEMORY_PREFIX[] = "QSynthesis_Memory_";

    const char LOCAL_SERVER_NAME[] = "QSynthesis_Local_Host_V2";

    // File and dir names
    const char FILE_NAME_RECORD_JSON[] = "record.json";
    const char FILE_NAME_SETTING_JSON[] = "config.json";
    const char FILE_NAME_KEYBOARD_JSON[] = "keyboard.json";

    const char FILE_NAME_HISTORY_DAT[] = "history.dat";
    const char FILE_NAME_INFO_JSON[] = "info.json";

    const char DIR_NAME_TEMP_CACHE[] = "cache";

    const char DIR_NAME_CONFIG[] = "config";
    const char DIR_NAME_CONFIG_FONTS[] = "fonts";
    const char DIR_NAME_CONFIG_THEMES[] = "themes";
    const char DIR_NAME_CONFIG_TOOLS[] = "tools";

    // Supplement of Utau
    const char KEY_NAME_PLUGIN_CHARSET[] = "charset";

} // namespace Qs

#endif // NAMESPACE_H
