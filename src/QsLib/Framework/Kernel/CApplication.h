#ifndef CAPPLICATION_H
#define CAPPLICATION_H

#include <QApplication>
#include <QDebug>
#include <QObjectList>
#include <QPair>

#include "QsFrameworkGlobal.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<CApplication *>(QCoreApplication::instance()))

class CApplicationPrivate;

class QSFRAMEWORK_API CApplication : public QApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CApplication)
public:
    explicit CApplication(int &argc, char **argv);
    ~CApplication();

public:
    static QString mainTitle();
    static QString windowTitle();
    static QString errorTitle();

    static QString sysFileManagerName();
    static QString sysRootUserName();
    static QString allFilesFilter();

public:
    QWidget *implicitMouseGrabber() const;

protected:
    CApplication(CApplicationPrivate &d, int &argc, char **argv);

    QScopedPointer<CApplicationPrivate> d_ptr;

protected:
    bool notify(QObject *obj, QEvent *event) override;

    virtual void newStartedInstance();
    virtual void receiveMessage(quint32 instanceId, const QByteArray &message);

private:
    void _q_instanceStarted();
    void _q_messageReceived(quint32 instanceId, QByteArray message);
};

#endif // CAPPLICATION_H
