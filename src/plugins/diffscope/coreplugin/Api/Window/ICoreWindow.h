#ifndef ICOREWINDOW_H
#define ICOREWINDOW_H

#include "IWindow.h"

namespace Core {

    class ICoreWindowPrivate;

    class CORE_EXPORT ICoreWindow : public IWindow {
        Q_OBJECT
    public:
        QMenuBar *menuBar() const override;
        void setMenuBar(QMenuBar *menuBar) override;

        QWidget *centralWidget() const override;
        void setCentralWidget(QWidget *widget) override;

        QStatusBar *statusBar() const override;
        void setStatusBar(QStatusBar *statusBar) override;

    protected:
        ICoreWindow(const QString &id, QObject *parent = nullptr);
        ~ICoreWindow();

        QWidget *createWindow(QWidget *parent) const override;

    protected:
        ICoreWindow(ICoreWindowPrivate &d, const QString &id, QObject *parent = nullptr);

        Q_DECLARE_PRIVATE(ICoreWindow)
    };

}



#endif // ICOREWINDOW_H
