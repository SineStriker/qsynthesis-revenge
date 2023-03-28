#include <QApplication>
#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QDebug>

#include "Collections/QMChronSet.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

//    QMChronSet<int> arr;
//    arr.append(2);
//    arr.append(1);
//    arr.append(3);
//
//    auto it = arr.find(1);
//    qDebug() << *std::next(it);
//    qDebug() << *std::prev(it);
//
//    arr.insert(std::next(arr.find(1)), 4);
//    qDebug() << arr;

    QString url = "http://qt-project.org";

    QNetworkAccessManager manager;

    // An highlighted block
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

    qDebug() << "Send HTTP request to:" << url;

    qDebug() << QByteArray("结果").toHex() << QString::fromUtf8("结果");

    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, &QIODevice::readyRead, [&]() {
        qDebug() << reply->readAll(); //
    });

    return a.exec();
}