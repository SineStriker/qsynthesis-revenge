#ifndef CHORUSKIT_MAINWINDOW_H
#define CHORUSKIT_MAINWINDOW_H

#include <QBuffer>
#include <QFile>
#include <QMainWindow>

#include <QCheckBox>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    QTextEdit *m_textEditTop;
    QTextEdit *m_textEditBottom;
    QPushButton *m_buttonConvert;
    QPushButton *m_buttonReorder;
    QRadioButton *m_radioBtnOption1;
    QRadioButton *m_radioBtnOption2;
    QButtonGroup *m_buttonGroup;
    QCheckBox *m_checkBox;
};

#endif // CHORUSKIT_MAINWINDOW_H
