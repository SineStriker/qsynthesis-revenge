#ifndef ACETREERECOVERABLE_H
#define ACETREERECOVERABLE_H

#include <QIODevice>
#include <QObject>

#include "QMEditingGlobal.h"

class AceTreeRecoverablePrivate;

class QMEDITING_EXPORT AceTreeRecoverable : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(AceTreeRecoverable)
public:
    ~AceTreeRecoverable();

public:
    void startLogging(QIODevice *dev);
    void stopLogging();
    bool isLogging() const;

    virtual bool recover(const QByteArray &data);

signals:
    void loggingError();

protected:
    AceTreeRecoverable(AceTreeRecoverablePrivate &d, QObject *parent = nullptr);

    QScopedPointer<AceTreeRecoverablePrivate> d_ptr;
};

#endif // ACETREERECOVERABLE_H