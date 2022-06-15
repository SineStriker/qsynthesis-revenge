#ifndef FILESACTIONENGINE_H
#define FILESACTIONENGINE_H

#include "ActionManagerEngine.h"

class FilesActionEnginePrivate;

class FilesActionEngine : public ActionManagerEngine {
    Q_DECLARE_PRIVATE(FilesActionEngine)
public:
    explicit FilesActionEngine(QObject *parent = nullptr);
    ~FilesActionEngine();

protected:
    FilesActionEngine(FilesActionEnginePrivate &d, QObject *parent = nullptr);

private:
    void q_actionTriggered(QAction *action);
};

#endif // FILESACTIONENGINE_H
