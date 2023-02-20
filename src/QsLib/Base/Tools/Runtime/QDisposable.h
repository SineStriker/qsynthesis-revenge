#ifndef QDISPOSABLE_H
#define QDISPOSABLE_H

#include <QObject>

#include "QsGlobal.h"

class QSBASE_API QDisposable : public QObject {
    Q_OBJECT
public:
    explicit QDisposable(QObject *parent = nullptr);
    ~QDisposable();

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

#endif // QDISPOSABLE_H
