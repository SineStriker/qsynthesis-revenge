#ifndef EXTENSIONSMANAGER_H
#define EXTENSIONSMANAGER_H

#include <QColor>
#include <QFile>

#include "BaseManager.h"
#include "Macros.h"

#define qTheme ExtensionsManager::instance()

class ExtensionsManagerPrivate;

class ExtensionsManager : public BaseManager {
    Q_OBJECT
    Q_SINGLETON(ExtensionsManager)
    Q_DECLARE_PRIVATE(ExtensionsManager)
public:
    explicit ExtensionsManager(QObject *parent = nullptr);
    ~ExtensionsManager();

public:
    bool load() override;
    bool save() override;

public:
    void themeLoad(int index);
    int themeCount() const;
    QStringList themeNames() const;

protected:
    ExtensionsManager(ExtensionsManagerPrivate &d, QObject *parent = nullptr);
};

#endif // EXTENSIONSMANAGER_H
