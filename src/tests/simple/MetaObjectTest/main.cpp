#include <QApplication>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QMainWindow>
#include <QMessageBox>
#include <QMetaType>
#include <QPushButton>

#include "TestObject.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    qRegisterMetaType<Test::TestObject *>(); // this could be elsewhere of course

    qDebug() << Test::TestObject::staticMetaObject.className();

    auto metaObject = QMetaType::metaObjectForType(QMetaType::type("Test::TestObject*"));
    if (metaObject) {
        auto obj = metaObject->newInstance();
        if (obj) {
            qDebug() << obj;
        }
    }

    return 0;
}