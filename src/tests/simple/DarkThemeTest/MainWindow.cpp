#include "MainWindow.h"
#include "DarkTheme.h"

MainWindow::MainWindow(QWidget *parent) {
    ui_centralWidget = new QWidget(this);
    ui_layout = new QVBoxLayout(ui_centralWidget);
    ui_label = new QLabel(ui_centralWidget);

    ui_layout->addWidget(ui_label);

    ui_centralWidget->setLayout(ui_layout);
    setCentralWidget(ui_centralWidget);

    m_dt = new DarkTheme();

    connect(m_dt, &DarkTheme::themeChanged,
            this, &MainWindow::slot_updateTheme);

    _displayInfo(m_dt->isDarkTheme());
}

MainWindow::~MainWindow() {
    delete m_dt;
}

/*void MainWindow::changeEvent(QEvent *e) {
    if ( e->type() == QEvent::PaletteChange )
    {
        _displayInfo();
    }
    QMainWindow::changeEvent(e);
}*/

void MainWindow::_displayInfo(bool isDarkTheme) {
    QString osName, theme;

    osName = m_dt->getOSName();

    if (isDarkTheme) {
        theme = "dark theme";
    } else {
        theme = "light theme";
    }

    ui_label->setText(osName + ": " + theme);
}
void MainWindow::slot_updateTheme(bool isDarkTheme) {
    _displayInfo(isDarkTheme);
}
