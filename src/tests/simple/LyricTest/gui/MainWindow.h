#ifndef CHORUSKIT_MAINWINDOW_H
#define CHORUSKIT_MAINWINDOW_H

#include "G2pWidget.h"
#include "TableWidget.h"
#include <QBuffer>
#include <QFile>
#include <QMainWindow>
#include <QWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QTabWidget *m_tabWidget;
    TableWidget *m_tableWidget;
    G2pWidget *m_g2pWidget;
};

#endif // CHORUSKIT_MAINWINDOW_H
