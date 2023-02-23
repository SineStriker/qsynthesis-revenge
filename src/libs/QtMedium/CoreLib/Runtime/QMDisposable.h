#ifndef QMDISPOSABLE_H
#define QMDISPOSABLE_H

#include <QObject>

#include "QMGlobal.h"

class QMCORELIB_API QMDisposable : public QObject {
    Q_OBJECT
public:
    explicit QMDisposable(QObject *parent = nullptr);
    ~QMDisposable();

public:
    void load();
    void save();
    bool isLoaded() const;

protected:
    virtual void loadImpl();
    virtual void saveImpl();

private:
    bool m_loaded;
};

#endif // QMDISPOSABLE_H
