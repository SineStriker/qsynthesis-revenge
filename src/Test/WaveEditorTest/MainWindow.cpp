
#include "MainWindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    mWaveEditor = new WaveEditor(this);
    setCentralWidget(mWaveEditor->view());
    mWaveEditor->SetAudio(qApp->arguments().at(1));
}

MainWindow::~MainWindow() {
}

void MainWindow::paintEvent(QPaintEvent *event) {
    QMainWindow::paintEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    // mView->fitInView(mOverviewThumbnail.rect());
}
