#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CoreGlobal.h"
#include "Windows/PlainWindow.h"

namespace Core {

    namespace Internal {

        class CorePlugin;

        class MainWindow : public QsApi::PlainWindow {
            Q_OBJECT
        public:
            explicit MainWindow(QWidget *parent = nullptr);
            ~MainWindow();
        };
    }

}


#endif // MAINWINDOW_H
