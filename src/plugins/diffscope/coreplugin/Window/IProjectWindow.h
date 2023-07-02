#ifndef IPROJECTWINDOW_H
#define IPROJECTWINDOW_H

#include <QAbstractButton>
#include <QToolBar>

#include <CDockFrame.h>

#include "Document/DspxDocument.h"
#include "ICoreWindow.h"
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

        // UI
        QToolBar *mainToolbar() const;
        CDockFrame *mainDock() const;
        PianoRoll *pianoRoll() const;

    protected:
        void nextLoadingState(Core::IWindow::State destState) override;

    protected:
        IProjectWindow(IProjectWindowPrivate &d, QObject *parent = nullptr);
    };

}

#endif // IPROJECTWINDOW_H
