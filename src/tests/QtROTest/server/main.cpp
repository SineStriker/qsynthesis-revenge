#include "simpleswitch.h"
#include <QApplication>
#include <QLabel>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    auto win = new QMainWindow;
    auto label = new QLabel;
    win->setCentralWidget(label);
    label->setAlignment(Qt::AlignCenter);

    SimpleSwitch srcSwitch; // create simple switch

    QRemoteObjectHost srcNode(QUrl(QStringLiteral("local:77F6E993-671E-4283-99BE-C1CD1FF5CO9E"))); // create host node without Registry
    srcNode.enableRemoting(&srcSwitch); // enable remoting/sharing

    QObject::connect(&srcSwitch, &SimpleSwitch::newValue, [=](int val){
        label->setText(QString::number(val));
    });

    win->show();

    return a.exec();
}