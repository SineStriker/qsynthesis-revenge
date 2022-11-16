#ifndef QSABSTRACTDRAGPROXY_H
#define QSABSTRACTDRAGPROXY_H

#include <QObject>

#include "qsintegrate_global.h"

class QSINTEGRATE_API QsAbstractDragProxy : public QObject {
    Q_OBJECT
public:
    explicit QsAbstractDragProxy(QObject *parent = nullptr);
    ~QsAbstractDragProxy();

public:
    QWidget *activeArea() const;
    void setActiveArea(QWidget *activeArea);

protected:
    QWidget *m_activeArea;

signals:
};

#endif // QSABSTRACTDRAGPROXY_H
