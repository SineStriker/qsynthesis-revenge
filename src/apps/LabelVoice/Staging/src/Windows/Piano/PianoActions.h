#ifndef PIANOACTIONS_H
#define PIANOACTIONS_H

#include <QAction>
#include <QMenu>
#include <QObject>

#include "Kernel/Actions.h"

class PianoWindow;

class PianoActions : public QObject {
    Q_OBJECT
public:
    explicit PianoActions(PianoWindow *parent = nullptr);
    ~PianoActions();

    PianoWindow *window;

public:
    void setup();
    void reloadStrings() ;
    void reloadShortcuts() ;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    // File
    QMenu *recentMenu; // *

    QAction *file_newFile;
    QAction *file_openFile;
    QAction *file_closeFile;
    QAction *file_saveFile;
    QAction *file_saveAs;
    QAction *file_importFile;
    QAction *file_appendFile;
    QAction *file_exportFile;
    QAction *file_exit;

    // Edit
    QMenu *pasteMenu;
    QMenu *removeMenu;

    QAction *edit_undo;
    QAction *edit_redo;

    QAction *edit_cut;
    QAction *edit_copy;
    QAction *edit_paste;
    QAction *edit_pastePitch;

    QAction *edit_remove;
    QAction *edit_removePitch;

    QAction *edit_selectAll;
    QAction *edit_deselect;

    // Help
    QAction *help_options;
    QAction *help_aboutApp;
    QAction *help_aboutQt;

protected:
    QMap<QAction *, ActionImpl::Action> map;

    QAction *NewAction(ActionImpl::Action a, QObject *parent = nullptr);
    void registerHandler(QMenu *menu);

    void reloadRecentActions();
    void reloadRecentActionStrings();
    void handleRecentAction(QAction *action);

private:
    void _q_actionTriggered(QAction *action);

signals:
    void triggered(int actionId);
};

#endif // PIANOACTIONS_H
