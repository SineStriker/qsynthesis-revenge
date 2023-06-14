#include "simpleswitch.h"
#include <QTimer>

// constructor
SimpleSwitch::SimpleSwitch(QObject *parent) : SimpleSwitchSimpleSource(parent)
{
    qDebug() << "Source Node Started";
}

int SimpleSwitch::work(int a, int b) {
    auto sum = a + b;
    emit newValue(sum);
    return sum;

}
