#ifndef BASEDRAGPROXY_H
#define BASEDRAGPROXY_H

#include <QObject>

class BaseDragProxy : public QObject {
    Q_OBJECT
public:
    explicit BaseDragProxy(QObject *parent = nullptr);
    ~BaseDragProxy();

public:
    QWidget *activeArea() const;
    void setActiveArea(QWidget *activeArea);

protected:
    QWidget *m_activeArea;

signals:
};

#endif // BASEDRAGPROXY_H
