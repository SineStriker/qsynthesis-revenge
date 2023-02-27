#ifndef ACTIONS_H
#define ACTIONS_H

namespace ActionImpl {

    enum MainMenu {
        File,
        Edit,
        View,
        Modify,
        Help,
        NumMenus,
    };

    enum Action {
        File_New,
        File_Open,
        File_Close,
        File_Save,
        File_SaveAs,
        File_SaveAll,
        File_Import,
        File_Append,
        File_Export,
        File_Exit,

        Edit_Undo,
        Edit_Redo,
        Edit_Cut,
        Edit_Copy,
        Edit_Paste,
        Edit_PastePitch,
        Edit_Remove,
        Edit_RemovePitch,
        Edit_SelectAll,
        Edit_Deselect,

        Help_Options,
        Help_AboutApplication,
        Help_AboutQt,

        Special_Recent,
    };

};

#endif // ACTIONS_H
