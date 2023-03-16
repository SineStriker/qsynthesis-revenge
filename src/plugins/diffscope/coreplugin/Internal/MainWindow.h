#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CoreGlobal.h"
#include "Widgets/Windows/PlainWindow.h"

namespace Core {

    namespace Internal {
        class CorePlugin;

        class MainWindow : public PlainWindow {
        public:
            explicit MainWindow(QWidget *parent = nullptr);
            ~MainWindow();
        };
    }

}


#endif // MAINWINDOW_H
