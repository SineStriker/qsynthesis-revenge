#ifndef IPROJECTWINDOW_H
#define IPROJECTWINDOW_H

#include "Document/DspxDocument.h"
#include "ICoreWindow.h"

namespace Core {

    namespace Internal {
        class IProjectWindowFactory;
    }

    class IProjectWindowPrivate;

    class CORE_EXPORT IProjectWindow : public ICoreWindow {
        Q_OBJECT
    public:
        static inline QString WindowTypeID() {
            return "project";
        }

        DspxDocument *doc() const;

        QToolBar *mainToolbar() const;

    protected:
        explicit IProjectWindow(QObject *parent = nullptr);
        ~IProjectWindow();

        void setupWindow() override;
        void windowAddOnsFinished() override;

        void windowAboutToClose() override;

    protected:
        IProjectWindow(IProjectWindowPrivate &d, QObject *parent = nullptr);

        Q_DECLARE_PRIVATE(IProjectWindow)

        friend class Internal::IProjectWindowFactory;
    };

}

#endif // IPROJECTWINDOW_H
