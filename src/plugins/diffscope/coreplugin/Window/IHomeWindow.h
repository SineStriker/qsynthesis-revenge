#ifndef IHOMEWINDOW_H
#define IHOMEWINDOW_H

#include <QAbstractButton>

#include "ICoreWindow.h"

namespace Core {

    namespace Internal {
        class IHomeWindowFactory;
    }

    class IHomeWindowPrivate;

    class CORE_EXPORT IHomeWindow : public ICoreWindow {
        Q_OBJECT
    public:
        static inline QString WindowTypeID() {
            return "home";
        }

        /**
         * brief: Add a navigation widget
         *
         * @param w QWidget to show in tabs
         * @return Sidebar button handle
         */
        QAbstractButton *addNavWidget(QWidget *w);

    protected:
        explicit IHomeWindow(QObject *parent = nullptr);
        ~IHomeWindow();

        void setupWindow() override;
        void windowAddOnsFinished() override;

        void windowAboutToClose() override;

    protected:
        IHomeWindow(IHomeWindowPrivate &d, QObject *parent = nullptr);

        Q_DECLARE_PRIVATE(IHomeWindow)

        friend class Internal::IHomeWindowFactory;
    };

}


#endif // IHOMEWINDOW_H
