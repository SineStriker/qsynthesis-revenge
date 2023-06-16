#include "TestObject.h"

#include <QDebug>

namespace Test {

    TestObject::TestObject(QObject *parent) : QObject(parent) {
        qDebug() << "Test::TestObject create";
    }

    TestObject::~TestObject() {
        qDebug() << "Test::TestObject destroy";
    }
}