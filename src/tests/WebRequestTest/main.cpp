#include <QApplication>
#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

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