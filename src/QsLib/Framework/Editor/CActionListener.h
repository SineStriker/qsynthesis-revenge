#ifndef CACTIONLISTENER_H
#define CACTIONLISTENER_H

#include <QAbstractButton>
#include <QAction>
#include <QMap>

#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API CActionListener : public QObject {
    Q_OBJECT
public:
    explicit CActionListener(QObject *parent = nullptr);
    ~CActionListener();

    void setChecked(bool checked);
    void setEnabled(bool enabled);

    enum ControlMode {
        NoControl,
        Normal,
        Reverse,
    };

    struct Controller {
        ControlMode checkedStatus;
        ControlMode enabledStatus;
    };

    void addAction(QAction *action, const Controller &c);
    void removeAction(QAction *action);

    void addButton(QAbstractButton *button, const Controller &c);
    void removeButton(QAbstractButton *button);

    void addListener(CActionListener *listener, const Controller &c);
    void removeListener(CActionListener *listener);

protected:
    QMap<QAction *, Controller> actions;
    QMap<QAbstractButton *, Controller> buttons;
    QMap<CActionListener *, Controller> listeners;

signals:
};

#endif // CACTIONLISTENER_H
