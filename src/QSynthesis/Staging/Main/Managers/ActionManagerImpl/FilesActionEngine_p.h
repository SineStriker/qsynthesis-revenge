#ifndef FILESACTIONENGINEPRIVATE_H
#define FILESACTIONENGINEPRIVATE_H

#include "ActionManagerEngine_p.h"

#include "FilesActionEngine.h"

class FilesActionEnginePrivate : public ActionManagerEnginePrivate {
    Q_DECLARE_PUBLIC(FilesActionEngine)
public:
    FilesActionEnginePrivate();
    ~FilesActionEnginePrivate();

    void init();

    void setup() override;
    void reloadStrings() override;
    void reloadShortcuts() override;

    void registerHandler(QMenu *menu);

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *modifyMenu;
    QMenu *playMenu;
    QMenu *helpMenu;

    QActionGroup *cursorGroup;

    // File
    QMenu *recentMenu; // *
    QMenu *exportMenu;
    QMenu *preferencesMenu;

    QAction *file_newFile;
    QAction *file_newWindow;
    QAction *file_openFile;
    QAction *file_openFolder;
    QAction *file_saveFile;
    QAction *file_saveAs;
    QAction *file_saveAll;
    QAction *file_importFile;
    QAction *file_appendFile;
    QAction *file_fileSettings;
    QAction *file_closeFile;
    QAction *file_closeWindow;

    QAction *export_exportSelection;
    QAction *export_exportTrack;

    QAction *preference_settings;
    QAction *preference_keyShortcuts;
    QAction *preference_themes;
    QAction *preference_languages;

    // Edit
    QMenu *pasteMenu;
    QMenu *removeMenu;

    QAction *edit_undo;
    QAction *edit_redo;

    QAction *edit_copy;
    QAction *edit_cut;
    QAction *edit_paste;

    QAction *edit_pasteMode1;
    QAction *edit_pasteMode2;
    QAction *edit_pasteEnv;
    QAction *edit_pasteProps;

    QAction *edit_remove;

    QAction *edit_removePoints;

    QAction *edit_selectAll;
    QAction *edit_deselect;

    // View
    QMenu *appearanceMenu;
    QMenu *panelsMenu;
    QMenu *quantizationMenu;
    QMenu *playheadMenu;
    QMenu *displayMenu;
    QMenu *cursorMenu;

    QAction *appearance_toolBar;
    QAction *appearance_navBar;
    QAction *appearance_statusBar;
    QAction *appearance_panelBars;
    QAction *appearance_menuBar;

    QAction *panels_notePanel;
    QAction *panels_ctrlPanel;
    QAction *panels_pitchPanel;
    QAction *panels_trackPanel;
    QAction *panels_lyricsPanel;
    QAction *panels_statePanel;

    QAction *playhead_normal;
    QAction *playhead_center;

    QAction *display_note;
    QAction *display_pitch;
    QAction *display_env;

    QAction *cursor_select;
    QAction *cursor_sketch;
    QAction *cursor_free;

    // Modify
    QMenu *envelopeMenu;

    QMenu *buildInMenu; // *
    QMenu *pluginsMenu; // *

    QAction *modify_insertLyrics;
    QAction *modify_findReplace;
    QAction *modify_transpose;
    QAction *modify_octaveUp;
    QAction *modify_octaveDown;
    QAction *modify_removeRest;
    QAction *modify_insertRest;

    QAction *envelope_p2p3Fade;
    QAction *envelope_p1p4Fade;
    QAction *envelope_resetEnv;

    QAction *modify_lyricConfig;

    // Play
    QAction *play_play;
    QAction *play_stop;
    QAction *play_replay;
    QAction *play_moveStart;
    QAction *play_moveEnd;
    QAction *play_removeCache;
    QAction *play_exportAudio;

    // Help
    QAction *help_welcome;
    QAction *help_showActions;
    QAction *help_instructions;
    QAction *help_checkUpdate;
    QAction *help_aboutApp;
    QAction *help_aboutQt;
};

#endif // FILESACTIONENGINEPRIVATE_H
