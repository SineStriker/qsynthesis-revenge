#ifndef HOMERECENTWIDGET_H
#define HOMERECENTWIDGET_H

#include <QFrame>

#include "CTabButton.h"

class HomeRecentWidget : public QFrame {
    Q_OBJECT
public:
    explicit HomeRecentWidget(QWidget *parent = nullptr);
    ~HomeRecentWidget();

    CTabButton *tabButton;

signals:
};

#endif // HOMERECENTWIDGET_H
