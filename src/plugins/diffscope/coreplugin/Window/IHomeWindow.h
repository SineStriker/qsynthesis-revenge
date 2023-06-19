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
        Q_DECLARE_PRIVATE(IHomeWindow)
    public:
        static inline QString WindowTypeID() {
            return "home";
        }

        static IHomeWindow *instance();

        QAbstractButton *addNavWidget(QWidget *w);

        void removeNavWidget(QWidget *w);

    protected:
        explicit IHomeWindow(QObject *parent = nullptr);
        ~IHomeWindow();

        void setupWindow() override;
        void windowAddOnsFinished() override;

        void windowAboutToClose() override;

    protected:
        IHomeWindow(IHomeWindowPrivate &d, QObject *parent = nullptr);

        friend class Internal::IHomeWindowFactory;
    };

}


#endif // IHOMEWINDOW_H
