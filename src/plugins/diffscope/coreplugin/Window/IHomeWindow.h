#ifndef IHOMEWINDOW_H
#define IHOMEWINDOW_H

#include <QAbstractButton>

#include "ICoreWindow.h"

namespace Core {

    class IHomeWindowPrivate;

    class CORE_EXPORT IHomeWindow : public ICoreWindow {
        Q_OBJECT
        Q_DECLARE_PRIVATE(IHomeWindow)
    public:
        explicit IHomeWindow(QObject *parent = nullptr);
        ~IHomeWindow();

        static IHomeWindow *instance();

    public:
        QAbstractButton *addNavWidget(QWidget *w);
        void removeNavWidget(QWidget *w);

    protected:
        void nextLoadingState(IWindow::State nextState) override;

    protected:
        IHomeWindow(IHomeWindowPrivate &d, QObject *parent = nullptr);
    };

}


#endif // IHOMEWINDOW_H
