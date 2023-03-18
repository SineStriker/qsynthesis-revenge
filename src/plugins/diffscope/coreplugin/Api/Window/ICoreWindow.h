#ifndef ICOREWINDOW_H
#define ICOREWINDOW_H

#include "IWindow.h"

namespace Core {

    const char HOME_WINDOW_ID[] = "home";
    const char PROJECT_WINDOW_ID[] = "project";

    class CORE_EXPORT ICoreWindow : public IWindow {
        Q_OBJECT
    protected:
        explicit ICoreWindow(const QString &id, QObject *parent = nullptr);
        ~ICoreWindow();

    public:
        QMenuBar *menuBar() const override;
        void setMenuBar(QMenuBar *menuBar) override;

        QWidget *centralWidget() const override;
        void setCentralWidget(QWidget *widget) override;

        QStatusBar *statusBar() const override;
        void setStatusBar(QStatusBar *statusBar) override;

    protected:
        QWidget *createWindow(QWidget *parent) const override;
    };

}



#endif // ICOREWINDOW_H
