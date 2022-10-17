#ifndef TESTWINDOW_H
#define TESTWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLineEdit>

class TestWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit TestWindow(QWidget *parent = nullptr);
    ~TestWindow();

protected:
    QPushButton *conv_btn;
    QPushButton *play_btn;

    QLineEdit *text;

private:
    void _q_convButtonClicked();
    void _q_playButtonClicked();

signals:
};

#endif // TESTWINDOW_H
