#ifndef LOCALDECORATOR_H
#define LOCALDECORATOR_H

#include <QObject>

#include "qsframework_global.h"

class LocalDecorator : public QObject {
    Q_OBJECT
public:
    explicit LocalDecorator(QObject *parent = nullptr);
    ~LocalDecorator();

signals:
};

#endif // LOCALDECORATOR_H
