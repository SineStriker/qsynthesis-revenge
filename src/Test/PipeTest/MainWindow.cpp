#include "MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    pipenameLabel = new QLabel("Pipe Name");
    pipenameText = new QLineEdit();

    waveLabel = new QLabel("Wave Path");
    waveText = new QLineEdit();

    browseButton = new QPushButton("...");

    waveBrowseLayout = new QHBoxLayout();
    waveBrowseLayout->setMargin(0);
    waveBrowseLayout->addWidget(waveText);
    waveBrowseLayout->addWidget(browseButton);

    connectButton = new QPushButton("Connect");
    disconnectButton = new QPushButton("Disconnect");

    buttonsLayout = new QHBoxLayout();
    buttonsLayout->setMargin(0);
    buttonsLayout->addWidget(connectButton);
    buttonsLayout->addWidget(disconnectButton);

    mainLayout = new QVBoxLayout();
    mainLayout->addWidget(pipenameLabel);
    mainLayout->addWidget(pipenameText);
    mainLayout->addWidget(waveLabel);
    mainLayout->addLayout(waveBrowseLayout);
    mainLayout->addLayout(buttonsLayout);

    QWidget *w = new QWidget();
    w->setLayout(mainLayout);
    setCentralWidget(w);

    connect(browseButton, &QPushButton::clicked, this, &MainWindow::_q_browseButtonClicked);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::_q_connectButtonClicked);
    connect(disconnectButton, &QPushButton::clicked, this, &MainWindow::_q_disconnectButtonClicked);

    client = new QLocalSocket(this);
    connect(client, &QLocalSocket::connected, this, &MainWindow::_q_socketConnectSuccess);
    connect(client, &QLocalSocket::disconnected, this, &MainWindow::_q_socketDisconnectSuccess);
    connect(client, &QLocalSocket::errorOccurred, this, &MainWindow::_q_socketErrorOccured);

    connect(client, &QLocalSocket::readyRead, this, &MainWindow::_q_socketReadyRead);

    resize(750, 5);

    // Read history
    QFile file(qApp->applicationDirPath() + "/client.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data(file.readAll());
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(data, &err);
        if (err.error == QJsonParseError::NoError && doc.isObject()) {
            QJsonObject obj = doc.object();
            {
                auto it = obj.find("pipename");
                if (it != obj.end() && it->isString()) {
                    pipenameText->setText(it->toString());
                }
            }
            {
                auto it = obj.find("wave");
                if (it != obj.end() && it->isString()) {
                    waveText->setText(QDir::toNativeSeparators(it->toString()));
                }
            }
        }
        file.close();
    }
}

MainWindow::~MainWindow() {
    if (client->state() == QLocalSocket::ConnectedState) {
        tryDisconnect();
    }

    // Save history
    QFile file(qApp->applicationDirPath() + "/client.json");
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)) {
        QJsonDocument doc;
        QJsonObject obj{
            {"pipename", pipenameText->text()},
            {"wave", QDir::toNativeSeparators(waveText->text())},
        };
        doc.setObject(obj);
        file.write(doc.toJson());
        file.close();
    }
}

void MainWindow::tryDisconnect() {
    // Disconnect
    client->disconnectFromServer();
}

void MainWindow::_q_browseButtonClicked() {
    QString path = QFileDialog::getOpenFileName(this, "Browse", ".", "Wave Files(*.wav)");
    if (!path.isEmpty()) {
        waveText->setText(QDir::toNativeSeparators(path));
    }
}

void MainWindow::_q_connectButtonClicked() {
    if (client->state() != QLocalSocket::UnconnectedState) {
        QMessageBox::information(this, qAppName(), "Client is connected.");
        return;
    }

    QString pipe = pipenameText->text();
    QString path = waveText->text();

    if (!QFileInfo(path).isFile()) {
        QMessageBox::critical(this, qAppName(), "Not a file!");
        return;
    }
    if (pipe.isEmpty()) {
        QMessageBox::critical(this, qAppName(), "Empty pipe name!");
        return;
    }
    client->connectToServer(pipe);
}

void MainWindow::_q_disconnectButtonClicked() {
    if (client->state() != QLocalSocket::ConnectedState) {
        QMessageBox::information(this, qAppName(), "Client is not connected.");
        return;
    }

    tryDisconnect();
}

void MainWindow::_q_socketConnectSuccess() {
    qDebug() << "Connect to server success";

    QString path = waveText->text();
    client->write(path.toUtf8());
    client->write("\n");
}

void MainWindow::_q_socketDisconnectSuccess() {
    qDebug() << "Disconnect from server success.";
}

void MainWindow::_q_socketErrorOccured(QLocalSocket::LocalSocketError e) {
    QString msg;
    switch (e) {
        case QLocalSocket::ConnectionRefusedError:
            msg = "Connection Refused";
            break;
        case QLocalSocket::PeerClosedError:
            msg = "Remote Host Closed";
            break;
        case QLocalSocket::ServerNotFoundError:
            msg = "Server Not Found";
            break;
        case QLocalSocket::SocketAccessError:
            msg = "Socket Access Error";
            break;
        case QLocalSocket::SocketResourceError:
            msg = "Socket Resource Error";
            break;
        case QLocalSocket::SocketTimeoutError:
            msg = "Socket Timeout Error";
            break;
        case QLocalSocket::DatagramTooLargeError:
            msg = "Datagram Too Large";
            break;
        case QLocalSocket::ConnectionError:
            msg = "Connection Error";
            break;
        case QLocalSocket::UnsupportedSocketOperationError:
            msg = "Unsupported Socket Operation";
            break;
        case QLocalSocket::UnknownSocketError:
            msg = "Unknown Socket";
            break;
        case QLocalSocket::OperationError:
            msg = "Operation Error";
            break;
    }
    QMessageBox::critical(this, qAppName(), QString("Error: %1.").arg(msg));
}

void MainWindow::_q_socketReadyRead() {
    static int i = 0;

    qDebug() << i++ << client->readAll().size();
}
