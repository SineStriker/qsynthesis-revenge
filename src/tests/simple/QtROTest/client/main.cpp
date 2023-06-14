#include "CommunicationHelper.h"
#include "CommunicationHelper_p.h"
#include "rep_SimpleSwitch_replica.h"
#include <QCoreApplication>
#include <QEventLoop>
#include <QMainWindow>
#include <QMessageBox>
#include <QMutex>
#include <QThread>
#include <iostream>
#include <thread>

CommunicationHelper helper;

void anotherCallback() {
    auto ret = helper.invokeSync<int>([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(2, 2);
        ret.waitForFinished();
        return ret.returnValue();
    });
    std::cerr << ret << std::endl;
    ret = helper.invokeSync<int>([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(2, 2);
        ret.waitForFinished();
        return ret.returnValue();
    });
    std::cerr << ret << std::endl;
}

int main(int argc, char *argv[])
{
    helper.start();
    reconnect:
    if(helper.connect<SimpleSwitchReplica>("local:switch", 2000)) std::cerr << "connected" << std::endl;
    else {
        std::cerr << "connect timeout" << std::endl;
        goto reconnect;
    }
    helper.invokeAsync<int>([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(114, 514);
        ret.waitForFinished();
        return ret.returnValue();
    }, [](int val){ std::cerr << val << std::endl; });
    std::cerr << "aaaa" << std::endl;
    std::thread thread(&anotherCallback);
    thread.detach();
    auto ret = helper.invokeSync<int>([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(1, 2);
        ret.waitForFinished();
        return ret.returnValue();
    });
    std::cerr << ret << std::endl;
    ret = helper.invokeSync<int>([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(1, 2);
        ret.waitForFinished();
        return ret.returnValue();
    });
    std::cerr << ret << std::endl;
    ret = helper.invokeSync<int>([&](){
        auto ret = helper.replica<SimpleSwitchReplica>()->work(1, 2);
        ret.waitForFinished();
        return ret.returnValue();
    });
    std::cerr << ret << std::endl;
    return 0;
}