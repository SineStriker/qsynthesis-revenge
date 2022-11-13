#ifndef LINEARSCROLLAREA_H
#define LINEARSCROLLAREA_H

#include <QDebug>
#include <QEvent>

#include "CScrollArea.h"

#include "qsintegrate_global.h"

class QSINTEGRATE_API LinearScrollArea : public CScrollArea {
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
