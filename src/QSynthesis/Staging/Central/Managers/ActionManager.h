#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "BaseManager.h"

class ActionManagerPrivate;
class MainWindow;

class ActionManager : public BaseManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(ActionManager)
public:
    explicit ActionManager(MainWindow *parent = nullptr);
    ~ActionManager();

public:
    bool load() override;

protected:
    ActionManager(ActionManagerPrivate &d, QObject *parent = nullptr);

signals:
};

#endif // ACTIONMANAGER_H
