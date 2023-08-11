//
// Created by Crs_1 on 2023/8/12.
//

#include <QSystemSemaphoreExtended.h>
#include <QCoreApplication>
#include <QDebug>
#include <QThread>

int main(int argc, char **argv) {

    QCoreApplication a(argc, argv);

    QSystemSemaphoreExtended sem("2c5a947fec7e485b91a4576cccdb804c", 3, QSystemSemaphore::Create);

    qDebug() << sem.tryAcquire(0);
    qDebug() << sem.tryAcquire(114514);
    qDebug() << sem.acquire();

    qDebug() << sem.tryAcquire(2000);

    qDebug() << "test finished";

    return a.exec();
}