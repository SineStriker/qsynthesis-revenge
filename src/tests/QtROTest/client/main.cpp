#include "CommunicationHelper.h"
#include "rep_SimpleSwitch_replica.h"
#include <QCoreApplication>
#include <QEventLoop>
#include <QMutex>
#include <QThread>
#include <iostream>
#include <thread>

CommunicationHelper helper;

void anotherCallback() {
    auto ret = helper.invokeSync([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(2, 2);
        ret.waitForFinished();
        return QVariant(ret.returnValue());
    });
    std::cerr << ret.toInt() << std::endl;
    ret = helper.invokeSync([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(2, 2);
        ret.waitForFinished();
        return QVariant(ret.returnValue());
    });
    std::cerr << ret.toInt() << std::endl;
}

int main(int argc, char *argv[])
{
    helper.start();
    helper.connect<SimpleSwitchReplica>("local:switch");
    std::cerr << "connected" << std::endl;
    std::thread thread(&anotherCallback);
    thread.detach();
    auto ret = helper.invokeSync([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(1, 2);
        ret.waitForFinished();
        return QVariant(ret.returnValue());
    });
    std::cerr << ret.toInt() << std::endl;
    ret = helper.invokeSync([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(1, 2);
        ret.waitForFinished();
        return QVariant(ret.returnValue());
    });
    std::cerr << ret.toInt() << std::endl;
    ret = helper.invokeSync([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(1, 2);
        ret.waitForFinished();
        return QVariant(ret.returnValue());
    });
    std::cerr << ret.toInt() << std::endl;
    return 0;
}