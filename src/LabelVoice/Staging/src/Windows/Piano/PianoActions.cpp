#include "PianoActions.h"
#include "PianoWindow.h"

#include "CMenu.h"

#include <QCoreApplication>
#include <QMenuBar>

PianoActions::PianoActions(PianoWindow *parent) : QObject(parent), window(parent) {
}

PianoActions::~PianoActions() {
}

void PianoActions::setup() {
    auto menuBar = window->menuBar();

    // ----------------- Allocate -----------------
    fileMenu = new CMenu(menuBar);
    editMenu = new CMenu(menuBar);
    viewMenu = new CMenu(menuBar);
    helpMenu = new CMenu(menuBar);

    registerHandler(fileMenu);
    registerHandler(editMenu);
    registerHandler(viewMenu);
    registerHandler(helpMenu);

    // File
    recentMenu = new CMenu(fileMenu); // *

    registerHandler(recentMenu);

    file_newFile = NewAction(ActionImpl::File_New, fileMenu);
    file_openFile = NewAction(ActionImpl::File_Open, fileMenu);
    file_closeFile = NewAction(ActionImpl::File_Close, fileMenu);
    file_saveFile = NewAction(ActionImpl::File_Save, fileMenu);
    file_saveAs = NewAction(ActionImpl::File_SaveAs, fileMenu);
    file_importFile = NewAction(ActionImpl::File_Import, fileMenu);
    file_appendFile = NewAction(ActionImpl::File_Append, fileMenu);
    file_exportFile = NewAction(ActionImpl::File_Export, fileMenu);
    file_exit = NewAction(ActionImpl::File_Exit, fileMenu);

    // Edit
    pasteMenu = new CMenu(editMenu);
    removeMenu = new CMenu(editMenu);

    registerHandler(pasteMenu);
    registerHandler(removeMenu);

    edit_undo = NewAction(ActionImpl::Edit_Undo, editMenu);
    edit_redo = NewAction(ActionImpl::Edit_Redo, editMenu);

    edit_cut = NewAction(ActionImpl::Edit_Cut, editMenu);
    edit_copy = NewAction(ActionImpl::Edit_Copy, editMenu);
    edit_paste = NewAction(ActionImpl::Edit_Paste, editMenu);
    edit_remove = NewAction(ActionImpl::Edit_Remove, editMenu);

    edit_pastePitch = NewAction(ActionImpl::Edit_PastePitch, pasteMenu);
    edit_removePitch = NewAction(ActionImpl::Edit_RemovePitch, removeMenu);

    edit_selectAll = NewAction(ActionImpl::Edit_SelectAll, editMenu);
    edit_deselect = NewAction(ActionImpl::Edit_Deselect, editMenu);

    // View

    // Help
    help_options = NewAction(ActionImpl::Help_Options, helpMenu);
    help_aboutApp = NewAction(ActionImpl::Help_AboutApplication, helpMenu);
    help_aboutQt = NewAction(ActionImpl::Help_AboutQt, helpMenu);



    // ----------------- Setup -----------------
    // File
    fileMenu->addAction(file_newFile);
    fileMenu->addAction(file_openFile);
    fileMenu->addMenu(recentMenu);
    fileMenu->addAction(file_closeFile);
    fileMenu->addSeparator();
    fileMenu->addAction(file_saveFile);
    fileMenu->addAction(file_saveAs);
    fileMenu->addSeparator();
    fileMenu->addAction(file_importFile);
    fileMenu->addAction(file_appendFile);
    fileMenu->addAction(file_exportFile);
    fileMenu->addSeparator();
    fileMenu->addAction(file_exit);

    // Edit
    pasteMenu->addAction(edit_pastePitch);
    removeMenu->addAction(edit_removePitch);

    editMenu->addAction(edit_undo);
    editMenu->addAction(edit_redo);
    editMenu->addSeparator();
    editMenu->addAction(edit_cut);
    editMenu->addAction(edit_copy);
    editMenu->addAction(edit_paste);
    editMenu->addMenu(pasteMenu);
    editMenu->addSeparator();
    editMenu->addAction(edit_remove);
    editMenu->addMenu(removeMenu);
    editMenu->addSeparator();
    editMenu->addAction(edit_selectAll);
    editMenu->addAction(edit_deselect);

    // View

    // Help
    helpMenu->addAction(help_options);
    helpMenu->addSeparator();
    helpMenu->addSeparator();
    helpMenu->addAction(help_aboutApp);
    helpMenu->addAction(help_aboutQt);

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(editMenu);
    menuBar->addMenu(viewMenu);
    menuBar->addMenu(helpMenu);
}

void PianoActions::reloadStrings() {
    fileMenu->setTitle(tr("File(&F)"));
    editMenu->setTitle(tr("Edit(&E)"));
    viewMenu->setTitle(tr("View(&V)"));
    helpMenu->setTitle(tr("Help(&H)"));

    // File
    recentMenu->setTitle(tr("Recent")); // *

    file_newFile->setText(tr("New"));
    file_openFile->setText(tr("Open..."));
    file_closeFile->setText(tr("Close"));
    file_saveFile->setText(tr("Save"));
    file_saveAs->setText(tr("Save As..."));
    file_importFile->setText(tr("Import..."));
    file_appendFile->setText(tr("Append..."));
    file_exportFile->setText(tr("Export..."));
    file_exit->setText(tr("Exit"));

    // Edit
    pasteMenu->setTitle(tr("More Paste Options"));
    removeMenu->setTitle(tr("More Remove Options"));

    edit_undo->setText(tr("Undo"));
    edit_redo->setText(tr("Redo"));

    edit_cut->setText(tr("Cut"));
    edit_copy->setText(tr("Copy"));
    edit_paste->setText(tr("Paste"));
    edit_pastePitch->setText(tr("Paste Pitch"));

    edit_remove->setText(tr("Remove"));
    edit_removePitch->setText(tr("Remove Pitch"));

    edit_selectAll->setText(tr("Select All"));
    edit_deselect->setText(tr("Deselect"));

    // View

    // Help
    help_options->setText(tr("Options"));
    help_aboutApp->setText(tr("About %1").arg(qApp->applicationName()));
    help_aboutQt->setText(tr("About %1").arg("Qt"));

    reloadShortcuts();
}

void PianoActions::reloadShortcuts() {
    // File
    file_newFile->setShortcut(QKeySequence("Ctrl+N"));
    file_openFile->setShortcut(QKeySequence("Ctrl+O"));
    file_saveFile->setShortcut(QKeySequence("Ctrl+S"));
    file_saveAs->setShortcut(QKeySequence("Ctrl+Shift+S"));

    // Edit
    edit_undo->setShortcut(QKeySequence("Ctrl+Z"));
    edit_redo->setShortcut(QKeySequence("Ctrl+Shift+Z"));

    edit_copy->setShortcut(QKeySequence("Ctrl+C"));
    edit_cut->setShortcut(QKeySequence("Ctrl+X"));
    edit_paste->setShortcut(QKeySequence("Ctrl+V"));

    edit_remove->setShortcut(QKeySequence("Del"));

    edit_selectAll->setShortcut(QKeySequence("Ctrl+A"));

    // View

    // Help
}

QAction *PianoActions::NewAction(ActionImpl::Action a, QObject *parent) {
    auto action = new QAction(parent);
    map.insert(action, a);
    return action;
}

void PianoActions::registerHandler(QMenu *menu) {
    connect(menu, &QMenu::triggered, this, &PianoActions::_q_actionTriggered);
}

void PianoActions::reloadRecentActions() {
}

void PianoActions::reloadRecentActionStrings() {
}

void PianoActions::handleRecentAction(QAction *action) {
}

void PianoActions::_q_actionTriggered(QAction *action) {
    auto menu = qobject_cast<QMenu *>(sender());
    const auto &ws = action->associatedWidgets();

    if (ws.isEmpty() || ws.front() != menu) {
        return;
    }

    if (menu == recentMenu) {
        handleRecentAction(action);
    } else {
        auto it = map.find(action);
        if (it == map.end()) {
            return;
        }
        emit triggered(it.value());
    }
}
