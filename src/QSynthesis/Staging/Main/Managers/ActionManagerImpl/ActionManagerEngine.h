#ifndef ACTIONMANAGERENGINE_H
#define ACTIONMANAGERENGINE_H

#include <QAction>

class ActionManagerEnginePrivate;

class ActionManagerEngine : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(ActionManagerEngine)
public:
    explicit ActionManagerEngine(QObject *parent = nullptr);
    virtual ~ActionManagerEngine();

    friend class ActionManager;
    friend class ActionManagerPrivate;

public:
    void setup();
    void reloadStrings();
    void reloadShortcuts();

protected:
    ActionManagerEngine(ActionManagerEnginePrivate &d, QObject *parent = nullptr);

    QScopedPointer<ActionManagerEnginePrivate> d_ptr;
};

#endif // ACTIONMANAGERENGINE_H
