#ifndef SINGLEAPPHANDLE_H
#define SINGLEAPPHANDLE_H

#include <QObject>

/*
 *  Wrapper of https://github.com/itay-grudev/SingleApplication
 */

class SingleAppHandlePrivate;

class SingleAppHandle : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(SingleAppHandle)
public:
    SingleAppHandle(QObject *parent = nullptr);
    ~SingleAppHandle();

    void setup();

protected:
    SingleAppHandle(SingleAppHandlePrivate &d, QObject *parent = nullptr);

    QScopedPointer<SingleAppHandlePrivate> d_ptr;
};

#endif // SINGLEAPPHANDLE_H
