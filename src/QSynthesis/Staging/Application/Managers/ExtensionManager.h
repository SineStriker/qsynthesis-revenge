#ifndef EXTENSIONSMANAGER_H
#define EXTENSIONSMANAGER_H

#include <QColor>
#include <QFile>

#include "BaseManager.h"
#include "Macros.h"

#define qTheme ExtensionManager::instance()

class ExtensionManagerPrivate;

class ExtensionManager : public BaseManager {
    Q_OBJECT
    Q_SINGLETON(ExtensionManager)
    Q_DECLARE_PRIVATE(ExtensionManager)
public:
    explicit ExtensionManager(QObject *parent = nullptr);
    ~ExtensionManager();

public:
    bool load() override;
    bool save() override;

public:
    void themeLoad(int index);
    int themeCount() const;
    QStringList themeNames() const;

protected:
    ExtensionManager(ExtensionManagerPrivate &d, QObject *parent = nullptr);
};

#endif // EXTENSIONSMANAGER_H
