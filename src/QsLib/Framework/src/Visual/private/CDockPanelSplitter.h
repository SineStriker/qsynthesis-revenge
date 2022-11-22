#ifndef __CDOCKPANELSPLITTER_H__
#define __CDOCKPANELSPLITTER_H__

#include <QSplitter>

#include "qsframework_global.h"

class QSFRAMEWORK_API CDockPanelSplitter : public QSplitter {
    Q_OBJECT
public:
    explicit CDockPanelSplitter(QWidget *parent = nullptr);
    explicit CDockPanelSplitter(Qt::Orientation orient, QWidget *parent = nullptr);
    ~CDockPanelSplitter();

signals:
};

#endif // __CDOCKPANELSPLITTER_H__