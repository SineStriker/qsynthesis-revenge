#ifndef CSCROLLAREA_H
#define CSCROLLAREA_H

#include <QScrollArea>

#include "qsframework_global.h"

class QSFRAMEWORK_API CScrollArea : public QScrollArea {
    Q_OBJECT
public:
    explicit CScrollArea(QWidget *parent = nullptr);
    ~CScrollArea();

public:
    void setValueX(int value);
    int valueX() const;

    void setValueY(int value);
    int valueY() const;

signals:
};

#endif // CSCROLLAREA_H
