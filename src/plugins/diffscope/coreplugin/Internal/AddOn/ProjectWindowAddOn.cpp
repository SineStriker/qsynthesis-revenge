#include "ProjectWindowAddOn.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QLabel>
#include <QMenuBar>
#include <QPushButton>
#include <QStatusBar>

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
            auto iWin = windowHandle()->cast<IProjectWindow>();

            CoreWindowAddOn::initialize();

            initActions();

            // Add phoneme panel
            phonemePanel = new Internal::PhonemePanel();
            phonemeButton = iWin->addFloatingPanel("edit.phonemePanel", phonemePanel, nullptr);

            qIDec->installLocale(this, _LOC(ProjectWindowAddOn, this));
        }

        void ProjectWindowAddOn::extensionsInitialized() {
            CoreWindowAddOn::extensionsInitialized();
        }

        void ProjectWindowAddOn::reloadStrings() {
            editItem->setText(tr("&Edit"));
            viewItem->setText(tr("&View"));

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

            appearanceMenuItem->setText(tr("Appearance"));
            menuBarVisibleItem->setText(tr("Main Menu"));
            mainToolbarVisibleItem->setText(tr("Main Toolbar"));
            dockVisibleItem->setText(tr("Dock Panel Bars"));
            statusBarVisibleItem->setText(tr("Status Bar"));

            dockPanelsVisibilityMenuItem->setText(tr("Dock Panels"));

            floatingPanelsVisibilityMenuItem->setText(tr("Floating Panels"));
            phonemePanelVisibleItem->setText(tr("Phonemes"));

            playItem->setText(tr("Play"));
            playItem->setCommandCheckedDescription(qMakePair(tr("Play"), tr("Pause")));

            stopItem->setText(tr("Stop"));
            moveToStartItem->setText(tr("Move to Start"));
            moveToEndItem->setText(tr("Move to End"));

            metronomeItem->setText(tr("Metronome"));
            loopPlayItem->setText(tr("Loop Play"));

            phonemeButton->setText(tr("Phonemes"));
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

        static void connectVisibilityControlAction(QAction *action, QWidget *w, bool initState = true) {
            action->setCheckable(true);
            QObject::connect(action, &QAction::toggled, w, &QWidget::setVisible);
            action->setChecked(initState);
        }

        void ProjectWindowAddOn::initActions() {
            auto iWin = windowHandle()->cast<IProjectWindow>();
            auto win = iWin->window();

            editItem = new ActionItem("core.Edit", ICore::createCoreMenu(win), this);
            viewItem = new ActionItem("core.View", ICore::createCoreMenu(win), this);

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

            // View
            appearanceMenuItem = new ActionItem("core.Appearance", ICore::createCoreMenu(win), this);
            menuBarVisibleItem = new ActionItem("core.MenuBarVisible", new QAction(this), this);
            mainToolbarVisibleItem = new ActionItem("core.MainToolbarVisible", new QAction(this), this);
            dockVisibleItem = new ActionItem("core.DockVisible", new QAction(this), this);
            statusBarVisibleItem = new ActionItem("core.StatusBarVisible", new QAction(this), this);

            dockPanelsVisibilityMenuItem =
                new ActionItem("core.DockPanelsVisibility", ICore::createCoreMenu(win), this);

            floatingPanelsVisibilityMenuItem =
                new ActionItem("core.FloatingPanelsVisibility", ICore::createCoreMenu(win), this);
            phonemePanelVisibleItem = new ActionItem("core.PhonemePanelVisible", new QAction(this), this);

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

            connectVisibilityControlAction(menuBarVisibleItem->action(), iWin->menuBar());
            connectVisibilityControlAction(mainToolbarVisibleItem->action(), iWin->mainToolbar());
            connectVisibilityControlAction(dockVisibleItem->action(), iWin->mainDock());
            connectVisibilityControlAction(statusBarVisibleItem->action(), iWin->statusBar());

            phonemePanelVisibleItem->action()->setCheckable(true);
            iWin->addCheckable("edit.phonemePanel", phonemePanelVisibleItem->action());

            playControlGroupItem->actionGroup()->setExclusionPolicy(QActionGroup::ExclusionPolicy::None);
            playItem->action()->setCheckable(true);

            playAssistGroupItem->actionGroup()->setExclusionPolicy(QActionGroup::ExclusionPolicy::None);
            metronomeItem->action()->setCheckable(true);
            loopPlayItem->action()->setCheckable(true);

            iWin->addCheckable("playback.playing", playItem->action());
            iWin->addCheckable("playback.metronome", metronomeItem->action());
            iWin->addCheckable("playback.loopPlay", loopPlayItem->action());

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

            iWin->addActionItems({
                editItem,
                viewItem,

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

                appearanceMenuItem,
                menuBarVisibleItem,
                mainToolbarVisibleItem,
                dockVisibleItem,
                statusBarVisibleItem,

                dockPanelsVisibilityMenuItem,

                floatingPanelsVisibilityMenuItem,
                phonemePanelVisibleItem,

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