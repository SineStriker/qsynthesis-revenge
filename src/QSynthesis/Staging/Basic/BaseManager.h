#ifndef BASEMANAGER_H
#define BASEMANAGER_H

#include <QObject>

class BaseManager : public QObject {
    Q_OBJECT
public:
    explicit BaseManager(QObject *parent = nullptr);
    ~BaseManager();

public:
    virtual bool load();
    virtual bool save();

signals:
};

#endif // BASEMANAGER_H
