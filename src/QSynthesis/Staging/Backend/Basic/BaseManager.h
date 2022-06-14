#ifndef BASEMANAGER_H
#define BASEMANAGER_H

#include <QObject>

class BaseManagerPrivate;

class BaseManager : public QObject {
    Q_OBJECT
public:
    explicit BaseManager(QObject *parent = nullptr);
    ~BaseManager();

public:
    virtual bool load();
    virtual bool save();

protected:
    BaseManager(BaseManagerPrivate &d, QObject *parent = nullptr);

    QScopedPointer<BaseManagerPrivate> d_ptr;

signals:
};

#endif // BASEMANAGER_H
