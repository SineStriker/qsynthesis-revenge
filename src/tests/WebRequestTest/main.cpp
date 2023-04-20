#include <QApplication>
#include <QMainWindow>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QDebug>

#include "Collections/QMChronMap.h"
#include "Collections/QMChronSet.h"

template <class T>
bool moveArr(QVector<T> arr, int index, int count, int dest) {
    count = qMin(count, arr.size() - index);
    if (count <= 0 || count > arr.size() || (dest >= index && dest < index + count)) {
        return false;
    }

    decltype(typename std::remove_reference<decltype(arr)>::type()) tmp;
    tmp.resize(count);
    std::copy(arr.begin() + index, arr.begin() + index + count, tmp.begin());

    // Do change
    int correctDest;
    if (dest > index) {
        correctDest = dest - count;
        auto sz = correctDest - index;
        for (int i = 0; i < sz; ++i) {
            arr[index + i] = arr[index + count + i];
        }
    } else {
        correctDest = dest;
        auto sz = index - dest;
        for (int i = sz - 1; i >= 0; --i) {
            arr[dest + count + i] = arr[dest + i];
        }
    }
    std::copy(tmp.begin(), tmp.end(), arr.begin() + correctDest);

    return true;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QVector<int> arr0{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    moveArr(arr0, 2, 10000, 0);

    {
        QMChronSet<int> arr;
        arr.append(2);
        arr.append(1);
        arr.append(3);

        auto it = arr.find(1);
        qDebug() << *std::next(it);
        qDebug() << *std::prev(it);

        arr.insert(std::next(arr.find(1)), 4);
        qDebug() << arr;

        qDebug() << *(arr.erase(arr.find(4)));
    }

    QMChronMap<QString, QString> map;
    map.append("file", "1");
    map.append("edit", "2");
    for (auto it = map.begin(); it != map.end();) {
        it = map.erase(it);
    }
    qDebug() << map;

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