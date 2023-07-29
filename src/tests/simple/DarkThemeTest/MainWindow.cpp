#include "MainWindow.h"
#include "DarkTheme.h"

MainWindow::MainWindow(QWidget *parent) {
    ui_centralWidget = new QWidget(this);
    ui_layout = new QVBoxLayout(ui_centralWidget);
    ui_label = new QLabel(ui_centralWidget);

    ui_layout->addWidget(ui_label);

    ui_centralWidget->setLayout(ui_layout);
    setCentralWidget(ui_centralWidget);

    _displayInfo();
}

MainWindow::~MainWindow() {}

void MainWindow::changeEvent(QEvent *e) {
    if ( e->type() == QEvent::PaletteChange )
    {
        _displayInfo();
    }
    QMainWindow::changeEvent(e);
}

void MainWindow::_displayInfo() {
    QString osName, theme;

#if defined(Q_OS_MAC)
    osName = "macOS";
#elif defined(Q_OS_WIN)
    osName = "Windows";
#elif defined(Q_OS_LINUX)
    osName = "Linux";
#else
    osName = "Other system";
#endif

    if (isDarkTheme()) {
        theme = "dark theme";
    } else {
        theme = "light theme";
    }

    ui_label->setText(osName + ": " + theme);
}
