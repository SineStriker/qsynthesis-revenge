#ifndef LINEARSCROLLAREA_H
#define LINEARSCROLLAREA_H

#include <QScrollArea>

#include "QsFrameworkGlobal.h"

class QSFRAMEWORK_API LinearScrollArea : public QScrollArea {
    Q_OBJECT
public:
    explicit LinearScrollArea(Qt::Orientation ori, QWidget *parent = nullptr);
    ~LinearScrollArea();

public:
    Qt::Orientation orient() const;

private:
    Qt::Orientation m_orient;

private:
    void resizeEvent(QResizeEvent *event) override;
};

#endif // LINEARSCROLLAREA_H
