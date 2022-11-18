#ifndef CAPPLICATION_H
#define CAPPLICATION_H

#include <QDebug>
#include <QObjectList>
#include <QPair>
#include <QApplication>

#include "qsframework_global.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<CApplication *>(QCoreApplication::instance()))

class CAppNotifyFilter;
class CApplicationPrivate;

class QSFRAMEWORK_API CApplication : public QApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CApplication)
public:
    CApplication(int &argc, char **argv);
    ~CApplication();

    void reloadStrings(int locale);
    void reloadScreen(int theme);

public:
    QWidget *implicitMouseGrabber() const;

    QObjectList notifyFilters() const;

    // This connection doesn't break when destroyed, you should disconnect manually in destructor
    bool installNotifyFilter(CAppNotifyFilter *nf);

    bool removeNotifyFilter(QObject *obj);
    void removeAllNotifyFilters();

protected:
    CApplication(CApplicationPrivate &d, int &argc, char **argv);

    QScopedPointer<CApplicationPrivate> d_ptr;

protected:
    bool notify(QObject *obj, QEvent *event) override;

private:
    void _q_instanceStarted();
    void _q_messageReceived(quint32 instanceId, QByteArray message);

signals:
};

// This feature has been deprecated
class CAppNotifyFilter {
public:
    QObject *obj;
    CAppNotifyFilter(QObject *obj) : obj(obj){};
    virtual ~CAppNotifyFilter(){};
    virtual bool notifyFilter(QObject *obj, QEvent *event) {
        Q_UNUSED(obj)
        Q_UNUSED(event)
        return false;
    }
};

#endif // CAPPLICATION_H
