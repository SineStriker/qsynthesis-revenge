#ifndef MODELHANDLER_H
#define MODELHANDLER_H

#include <QObject>

class ModelHandler : public QObject {
    Q_OBJECT
public:
    explicit ModelHandler(QObject *parent = nullptr);
    ~ModelHandler();

signals:
};

#endif // MODELHANDLER_H
