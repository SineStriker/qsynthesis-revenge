#include "rep_SimpleSwitch_replica.h"
#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSharedPointer<SimpleSwitchReplica> ptr; // shared pointer to hold source replica

    QRemoteObjectNode repNode; // create remote object node
    repNode.connectToNode(QUrl(QStringLiteral("local:switch"))); // connect with remote host node

    ptr.reset(repNode.acquire<SimpleSwitchReplica>()); // acquire replica of source from host node

    auto win = new QMainWindow;
    auto widget = new QWidget;
    auto layout = new QVBoxLayout;
    auto sb1 = new QSpinBox;
    auto sb2 = new QSpinBox;
    auto btn = new QPushButton;
    layout->addWidget(sb1);
    layout->addWidget(sb2);
    layout->addWidget(btn);
    widget->setLayout(layout);
    win->setCentralWidget(widget);
    QObject::connect(btn, &QPushButton::clicked, [=](){
        auto ret = ptr->work(sb1->value(), sb2->value());
        ret.waitForFinished();
        qDebug() << ret.returnValue();
    });

    QObject::connect(ptr.data(), &SimpleSwitchReplica::initialized, [=](){
        win->show();
    });

    QObject::connect(ptr.data(), &SimpleSwitchReplica::stateChanged, [=](SimpleSwitchReplica::State state, SimpleSwitchReplica::State oldState) {
        QMessageBox::information(win, "State Changed", QString("New state is %1, old state is %2.").arg(state).arg(oldState));
    });

    return a.exec();
}