#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QFuture>
#include <QJSEngine>
#include <QThread>
#include <QtConcurrent>

QString func2(QString name) {
    qDebug() << name << "from" << QThread::currentThread();
    return name;
}

int main(int argc, char *argv[]) {
    // Core, GUI
    QApplication a(argc, argv);

    // Qml
    QJSEngine myEngine;
    QJSValue three = myEngine.evaluate("1 + 2");
    qDebug() << "JSEngine result:" << three.toInt(); // 3

    // Network
    QString url = "http://qt-project.org";

    QNetworkAccessManager manager;

    // A highlighted block
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setRawHeader("User-Agent", "MyOwnBrowser 1.0");

    qDebug() << "Send HTTP request to:" << url;

    QNetworkReply *reply = manager.get(request);
    QObject::connect(reply, &QIODevice::readyRead, [&]() {
        qDebug() << reply->readAll(); //
    });

    QFuture<QString> fut2 = QtConcurrent::run(func2, QString("Thread 1")); // 2.参数2：向func函数传递的参数
    QString result2 = fut2.result();
    fut2.waitForFinished();

    return a.exec();
}