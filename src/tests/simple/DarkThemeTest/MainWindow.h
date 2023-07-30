#ifndef DARKTHEME_MAINWINDOW_H
#define DARKTHEME_MAINWINDOW_H

#include <QEvent>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include "DarkTheme.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    /*void changeEvent(QEvent *e) override;*/
    void slot_updateTheme(bool isDarkTheme);
private:
    QWidget *ui_centralWidget;
    QVBoxLayout *ui_layout;
    QLabel *ui_label;

    DarkTheme *m_dt;
    void _displayInfo(bool isDarkTheme);
};

#endif // DARKTHEME_MAINWINDOW_H
