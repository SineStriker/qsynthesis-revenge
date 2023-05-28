#include "ProjectWindowAddOn.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>

#include <QMDecoratorV2.h>
#include <QMSystem.h>

#include "CoreApi/IWindow.h"
#include "ICore.h"
#include "Window/IProjectWindow.h"

#include "Internal/Utils/LastWindowFilter.h"

namespace Core {

    namespace Internal {

        bool ProjectWindowAddOnFactory::predicate(IWindow *handle) const {
            return handle->id() == "project";
        }

        IWindowAddOn *ProjectWindowAddOnFactory::create(QObject *parent) {
            return new ProjectWindowAddOn(parent);
        }

        ProjectWindowAddOn::ProjectWindowAddOn(QObject *parent) : CoreWindowAddOn(parent) {
        }

        ProjectWindowAddOn::~ProjectWindowAddOn() {
        }

        void ProjectWindowAddOn::initialize() {
            CoreWindowAddOn::initialize();

            initActions();

            qIDec->installLocale(this, _LOC(ProjectWindowAddOn, this));
        }

        void ProjectWindowAddOn::extensionsInitialized() {
            CoreWindowAddOn::extensionsInitialized();
        }

        void ProjectWindowAddOn::reloadStrings() {
            editItem->setText(tr("&Edit"));

            saveGroupItem->setText(tr("Save Actions"));
            saveFileItem->setText(tr("&Save"));
            saveAsFileItem->setText(tr("Save &As..."));

            exitGroupItem->setText(tr("Exit Actions"));
            closeFileItem->setText(tr("Close"));

            undoGroupItem->setText(tr("Undo Actions"));
            undoItem->setText(tr("&Undo"));
            redoItem->setText(tr("&Redo"));

            editGroupItem->setText(tr("Edit Actions"));
            cutItem->setText(tr("Cu&t"));
            copyItem->setText(tr("&Copy"));
            pasteItem->setText(tr("&Paste"));
            deleteItem->setText(tr("&Delete"));

            selectGroupItem->setText(tr("Select Actions"));
            selectAllItem->setText(tr("Select &All"));
            deselectItem->setText(tr("Deselect"));

            playItem->setText(tr("Play"));
            playItem->setCommandCheckedDescription(qMakePair(tr("Play"), tr("Pause")));

            stopItem->setText(tr("Stop"));
            moveToStartItem->setText(tr("Move to Start"));
            moveToEndItem->setText(tr("Move to End"));

            metronomeItem->setText(tr("Metronome"));
            loopPlayItem->setText(tr("Loop Play"));
        }

        static QAction *createToolBarAction(QObject *parent, const QString &name, bool selectable = false) {
            auto action = new QAction(parent);
            action->setObjectName(name);
            if (selectable) {
                action->setProperty("selectable", true);
            }
            return action;
        }

        static QWidget *createStretch() {
            auto w = new QWidget();
            w->setDisabled(true);
            w->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            return w;
        }

        void ProjectWindowAddOn::initActions() {
            auto iWin = windowHandle()->cast<IProjectWindow>();
            auto win = iWin->window();

            editItem = new ActionItem("core.Edit", ICore::createCoreMenu(win), this);

            // File
            saveGroupItem = new ActionItem("core.SaveGroup", new QActionGroup(this), this);
            saveFileItem = new ActionItem("core.SaveFile", new QAction(this), this);
            saveAsFileItem = new ActionItem("core.SaveAsFile", new QAction(this), this);

            exitGroupItem = new ActionItem("core.ExitGroup", new QActionGroup(this), this);
            closeFileItem = new ActionItem("core.CloseFile", new QAction(this), this);

            // Edit
            undoGroupItem = new ActionItem("core.UndoGroup", new QActionGroup(this), this);
            undoItem = new ActionItem("core.Undo", new QAction(this), this);
            redoItem = new ActionItem("core.Redo", new QAction(this), this);

            editGroupItem = new ActionItem("core.EditGroup", new QActionGroup(this), this);
            cutItem = new ActionItem("core.Cut", new QAction(this), this);
            copyItem = new ActionItem("core.Copy", new QAction(this), this);
            pasteItem = new ActionItem("core.Paste", new QAction(this), this);
            deleteItem = new ActionItem("core.Delete", new QAction(this), this);

            selectGroupItem = new ActionItem("core.SelectGroup", new QActionGroup(this), this);
            selectAllItem = new ActionItem("core.SelectAll", new QAction(this), this);
            deselectItem = new ActionItem("core.Deselect", new QAction(this), this);

            // Toolbar
            timerGroupItem = new ActionItem("core.TimerGroup", new QActionGroup(this), this);
            timerLabelItem = new ActionItem("core.TimerLabel", new QLabel("00:00.000"), this);

            quantizeGroupItem = new ActionItem("core.QuantizeGroup", new QActionGroup(this), this);
            quantizeSelectorItem = new ActionItem("core.QuantizeSelector", new QLabel("1/4"), this);

            playControlGroupItem = new ActionItem("core.PlayControlGroup", new QActionGroup(this), this);
            playItem = new ActionItem("core.Play", createToolBarAction(this, "play"), this);
            stopItem = new ActionItem("core.Stop", createToolBarAction(this, "stop"), this);
            moveToStartItem = new ActionItem("core.MoveToStart", createToolBarAction(this, "moveToStart"), this);
            moveToEndItem = new ActionItem("core.MoveToEnd", createToolBarAction(this, "moveToEnd"), this);

            playAssistGroupItem = new ActionItem("core.PlayAssistGroup", new QActionGroup(this), this);
            metronomeItem = new ActionItem("core.Metronome", createToolBarAction(this, "metronome", true), this);
            loopPlayItem = new ActionItem("core.LoopPlay", createToolBarAction(this, "loopPlay", true), this);

            // Stretch Item
            mainToolbarStretchItem = new ActionItem("core.MainToolbarStretch", createStretch(), this);
            mainToolbarStretchItem->setProperty("is-stretch", true);

            mainToolbarHelpGroupItem = new ActionItem("core.MainToolbarHelpGroup", new QActionGroup(this), this);

            // Set properties
            undoItem->setProperty("no-command-palette", true);
            redoItem->setProperty("no-command-palette", true);

            playControlGroupItem->actionGroup()->setExclusionPolicy(QActionGroup::ExclusionPolicy::None);
            playItem->action()->setCheckable(true);

            playAssistGroupItem->actionGroup()->setExclusionPolicy(QActionGroup::ExclusionPolicy::None);
            metronomeItem->action()->setCheckable(true);
            loopPlayItem->action()->setCheckable(true);

            connect(iWin->doc(), &IDocument::changed, this, [this, iWin]() {
                saveFileItem->setEnabled(iWin->doc()->isModified()); //
            });

            connect(saveAsFileItem->action(), &QAction::triggered, this, [this, iWin]() {
                auto doc = iWin->doc();
                auto filePath = doc->filePath();
                if (!QMFs::isFileExist(filePath)) {
                    saveAsFileItem->action()->trigger();
                } else {
                    ICore::instance()->documentSystem()->saveDocument(iWin->doc());
                }
            });

            connect(saveAsFileItem->action(), &QAction::triggered, this, [this, iWin]() {
                ICore::instance()->documentSystem()->saveFileBrowse(iWin->window(), iWin->doc()); //
            });

            connect(closeFileItem->action(), &QAction::triggered, this, [this, win]() {
                LastWindowFilter guard;
                win->close();
            });

            connect(stopItem->action(), &QAction::triggered, this, [this, iWin]() {
                iWin->requestGlobalEvent("playback.stop");
                iWin->setGlobalAttribute("playback.playing", false);
            });

            connect(moveToStartItem->action(), &QAction::triggered, this, [this, iWin]() {
                iWin->requestGlobalEvent("playback.moveToStart"); //
            });

            connect(moveToEndItem->action(), &QAction::triggered, this, [this, iWin]() {
                iWin->requestGlobalEvent("playback.moveToEnd"); //
            });

            iWin->addCheckable("playback.playing", playItem->action());
            iWin->addCheckable("playback.metronome", metronomeItem->action());
            iWin->addCheckable("playback.loopPlay", loopPlayItem->action());

            iWin->addActionItems({
                editItem,

                saveGroupItem,
                saveFileItem,
                saveAsFileItem,

                exitGroupItem,
                closeFileItem,

                undoGroupItem,
                undoItem,
                redoItem,

                editGroupItem,
                cutItem,
                copyItem,
                pasteItem,
                deleteItem,

                selectGroupItem,
                selectAllItem,
                deselectItem,

                timerGroupItem,
                timerLabelItem,

                quantizeGroupItem,
                quantizeSelectorItem,

                playControlGroupItem,
                playItem,
                stopItem,
                moveToStartItem,
                moveToEndItem,

                playAssistGroupItem,
                metronomeItem,
                loopPlayItem,

                mainToolbarStretchItem,
                mainToolbarHelpGroupItem,
            });
        }
    }

}