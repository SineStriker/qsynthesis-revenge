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
            saveGroupItem->setText(tr("Save Actions"));
            saveFileItem->setText(tr("&Save"));
            saveAsFileItem->setText(tr("Save &As..."));

            exitGroupItem->setText(tr("Exit Actions"));
            closeFileItem->setText(tr("Close"));

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

            saveGroupItem = new ActionItem("core.SaveGroup", new QActionGroup(this), this);
            saveFileItem = new ActionItem("core.SaveFile", new QAction(this), this);
            saveAsFileItem = new ActionItem("core.SaveAsFile", new QAction(this), this);

            exitGroupItem = new ActionItem("core.ExitGroup", new QActionGroup(this), this);
            closeFileItem = new ActionItem("core.CloseFile", new QAction(this), this);

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
            mainToolbarStretchItem->setProperty("start-of-layout", true);

            mainToolbarHelpGroupItem = new ActionItem("core.MainToolbarHelpGroup", new QActionGroup(this), this);

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
                saveGroupItem,
                saveFileItem,
                saveAsFileItem,

                exitGroupItem,
                closeFileItem,

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