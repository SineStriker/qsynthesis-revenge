#ifndef SIMPLESWITCH_H
#define SIMPLESWITCH_H

#include "rep_SimpleSwitch_source.h"

class SimpleSwitch : public SimpleSwitchSimpleSource
{
    Q_OBJECT
public:
    SimpleSwitch(QObject *parent = nullptr);
    int work(int a, int b) override;
signals:
    void newValue(int val);
};

#endif