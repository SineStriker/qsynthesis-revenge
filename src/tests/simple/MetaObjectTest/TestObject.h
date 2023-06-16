#ifndef CHORUSKIT_TESTOBJECT_H
#define CHORUSKIT_TESTOBJECT_H

#include <QObject>

namespace Test {

    class TestObject : public QObject {
        Q_OBJECT
    public:
        Q_INVOKABLE explicit TestObject(QObject *parent = nullptr);
        ~TestObject();
    };

}

#endif // CHORUSKIT_TESTOBJECT_H
