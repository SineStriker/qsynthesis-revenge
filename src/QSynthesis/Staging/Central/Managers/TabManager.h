#ifndef TABMANAGER_H
#define TABMANAGER_H

#include "BaseManager.h"

class MainWindow;
class TabManagerPrivate;

class TabManager : public BaseManager {
    Q_OBJECT
    Q_DECLARE_PRIVATE(TabManager)
public:
    explicit TabManager(MainWindow *parent = nullptr);
    ~TabManager();

public:
    bool load() override;

protected:
    TabManager(TabManagerPrivate &d, QObject *parent = nullptr);

    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    void _q_tabCloseRequested(int index);
    void _q_tabIndexChanged(int index, int orgIndex);
    void _q_tabTitleChanged(const QString &title);
    void _q_tabBarClicked(Qt::MouseButton button, int index);

signals:
};

#endif // TABMANAGER_H
