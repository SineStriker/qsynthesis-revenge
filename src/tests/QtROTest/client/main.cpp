#include "CommunicationHelper.h"
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
    helper.connect<SimpleSwitchReplica>("local:switch", 2000, [](){
        std::cout << "Retry? ";
        std::string s;
        std::cin >> s;
        return s[0] == 'y';
    });
    std::cerr << "connected" << std::endl;
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