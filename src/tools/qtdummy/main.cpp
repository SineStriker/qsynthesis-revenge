#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>

int main(int argc, char *argv[]) {
    // Core, GUI
    QApplication a(argc, argv);

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

    return a.exec();
}