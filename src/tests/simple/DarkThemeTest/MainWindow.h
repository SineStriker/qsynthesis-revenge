#ifndef DARKTHEME_MAINWINDOW_H
#define DARKTHEME_MAINWINDOW_H

#include <QEvent>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void changeEvent(QEvent *e) override;
private:
    QWidget *ui_centralWidget;
    QVBoxLayout *ui_layout;
    QLabel *ui_label;

    void _displayInfo();
};

#endif // DARKTHEME_MAINWINDOW_H
