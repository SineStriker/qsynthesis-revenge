#ifndef IPROJECTWINDOW_H
#define IPROJECTWINDOW_H

#include <QAbstractButton>
#include <QToolBar>

#include <CDockFrame.h>

#include "Document/DspxDocument.h"
#include "ICoreWindow.h"
#include "PianoRoll/MusicTimeManager.h"
#include "PianoRoll/PianoRoll.h"

namespace Core {

    class IProjectWindowPrivate;

    class CORE_EXPORT IProjectWindow : public ICoreWindow {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IProjectWindow)
    public:
        explicit IProjectWindow(DspxDocument *doc, QObject *parent = nullptr);
        ~IProjectWindow();

    public:
        // Meta
        DspxDocument *doc() const;
        MusicTimeManager *timeManager() const;

        // UI
        QToolBar *mainToolbar() const;
        CDockFrame *mainDock() const;
        PianoRoll *pianoRoll() const;

    public:

    protected:
        void nextLoadingState(IWindow::State nextState) override;

    protected:
        IProjectWindow(IProjectWindowPrivate &d, QObject *parent = nullptr);
    };

}

#endif // IPROJECTWINDOW_H
