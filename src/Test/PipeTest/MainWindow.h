#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QLabel>
#include <QLineEdit>
#include <QLocalSocket>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QThread>
#include <QLocalServer>

#include "WaveObject.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    QLabel *pipenameLabel, *waveLabel;
    QLineEdit *pipenameText, *waveText;

    QPushButton *browseButton;
    QPushButton *connectButton;
    QPushButton *disconnectButton;

    QHBoxLayout *waveBrowseLayout;
    QHBoxLayout *buttonsLayout;
    QVBoxLayout *mainLayout;

    QLocalSocket *client;
    QLocalServer *server;

    void tryDisconnect();

private:
    void _q_browseButtonClicked();
    void _q_connectButtonClicked();
    void _q_disconnectButtonClicked();

    void _q_socketConnectSuccess();
    void _q_socketDisconnectSuccess();
    void _q_socketErrorOccured(QLocalSocket::LocalSocketError e);

    void _q_socketReadyRead();
};
#endif // MAINWINDOW_H
