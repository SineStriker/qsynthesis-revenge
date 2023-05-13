//
// Created by Crs_1 on 2023/5/13.
//
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QProcess>

#include "IpcServer.h"
#include "IpcSendChannel.h"

using namespace Vst;

extern "C" Q_DECL_EXPORT void Initializer();
extern "C" Q_DECL_EXPORT void Terminator();

int main(int argc, char **argv) {
    QApplication a(argc, argv);
    auto *ipcServer = new IpcServer("77F6E993-671E-4283-99BE-C1CD1FF5C09E", nullptr);
    auto *ipcSendChannel = new IpcSendChannel(ipcServer);
    auto *w = new QMainWindow;
    auto *widget = new QWidget;
    auto *layout = new QVBoxLayout;
    auto *lineEdit = new QLineEdit;
    auto *btn = new QPushButton;
    QObject::connect(btn, &QPushButton::clicked, [=](){
        auto ret = ipcSendChannel->send(42, lineEdit->text().toLocal8Bit());
        qDebug() << ret;
    });
    layout->addWidget(lineEdit);
    layout->addWidget(btn);
    widget->setLayout(layout);
    w->setCentralWidget(widget);
    w->show();
    Initializer();
    a.exec();
}
