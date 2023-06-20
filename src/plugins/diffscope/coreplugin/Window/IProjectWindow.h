#ifndef IPROJECTWINDOW_H
#define IPROJECTWINDOW_H

#include <QAbstractButton>
#include <QToolBar>

#include <CDockFrame.h>

#include "Document/DspxDocument.h"
#include "ICoreWindow.h"
#include "PianoRoll/PianoRoll.h"

namespace Core {

    namespace Internal {
        class IProjectWindowFactory;
        class ProjectWindowAddOn;
    }

    class IProjectWindowPrivate;

    class CORE_EXPORT IProjectWindow : public ICoreWindow {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IProjectWindow)
    public:
        static inline QString WindowTypeID() {
            return "project";
        }

        DspxDocument *doc() const;

        QToolBar *mainToolbar() const;

        CDockFrame *mainDock() const;

        PianoRoll *pianoRoll() const;

    protected:
        explicit IProjectWindow(QObject *parent = nullptr);
        ~IProjectWindow();

        void setupWindow() override;
        void windowAddOnsFinished() override;

        void windowAboutToClose() override;

    protected:
        IProjectWindow(IProjectWindowPrivate &d, QObject *parent = nullptr);

        friend class Internal::IProjectWindowFactory;
    };

}

#endif // IPROJECTWINDOW_H
