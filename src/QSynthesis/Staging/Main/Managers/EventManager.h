#ifndef EVENTMANAGER_H
#define EVENTMANAGER_H

#include "../CentralManager.h"

class MainWindow;
class EventManagerPrivate;

class EventManager : public CentralManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(EventManager)
public:
    explicit EventManager(MainWindow *parent = nullptr);
    ~EventManager();

public:
    bool load() override;

public:
    bool import(const QString &filename);

protected:
    EventManager(EventManagerPrivate &d, MainWindow *parent = nullptr);

    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // EVENTMANAGER_H
