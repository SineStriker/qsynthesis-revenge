#ifndef CHORUSKIT_MAINWINDOW_H
#define CHORUSKIT_MAINWINDOW_H

#include <QBuffer>
#include <QFile>
#include <QMainWindow>

#include <Widgets/AceTreeWidget.h>

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QsApi::AceTreeWidget *m_treeWidget;

    QBuffer m_buffer;
    QByteArray m_data;

    QFile m_file;
};

#endif // CHORUSKIT_MAINWINDOW_H
