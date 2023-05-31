#ifndef IPROJECTWINDOW_H
#define IPROJECTWINDOW_H

#include <CDockFrame.h>

#include <QAbstractButton>

#include "Document/DspxDocument.h"
#include "ICoreWindow.h"
#include "Interfaces/IPianoKeyWidget.h"

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

        QAbstractButton *addFloatingPanel(const QString &id, QWidget *panel, QWidget *titleBar);

        QAbstractButton *addToolWindowPanel(const QString &id, int preferredEdge, QWidget *panel);

        void addPianoKeyWidget(const QString &id, IPianoKeyWidgetFactory *factory);

    protected:
        explicit IProjectWindow(QObject *parent = nullptr);
        ~IProjectWindow();

        void setupWindow() override;
        void windowAddOnsFinished() override;

        void windowAboutToClose() override;

    protected:
        IProjectWindow(IProjectWindowPrivate &d, QObject *parent = nullptr);

        friend class Internal::IProjectWindowFactory;

        friend class Internal::ProjectWindowAddOn;
    };

}

#endif // IPROJECTWINDOW_H
