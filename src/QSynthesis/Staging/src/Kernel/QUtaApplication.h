#ifndef QUTAAPPLICATION_H
#define QUTAAPPLICATION_H

#include "Kernel/QUtaElemApplication.h"

#ifdef qApp
#undef qApp
#endif
#define qApp (qobject_cast<QUtaApplication *>(QCoreApplication::instance()))

class QUtaApplicationPrivate;

class QUtaApplication : public QUtaElemApplication {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QUtaApplication)
public:
    QUtaApplication(int &argc, char **argv);
    ~QUtaApplication();

    void reloadStrings(int locale);
    void reloadScreen(int theme);

protected:
    void receiveMessage(quint32 instanceId, const QByteArray &message) override;

protected:
    QUtaApplication(QUtaApplicationPrivate &d, int &argc, char **argv);
};

#endif // QUTAAPPLICATION_H
