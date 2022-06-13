#ifndef CENTRALTABWIDGET_H
#define CENTRALTABWIDGET_H

#include "QScrollableTabWidget.h"

class CentralTabWidget : public QScrollableTabWidget {
    Q_OBJECT
public:
    explicit CentralTabWidget(QWidget *parent = nullptr);
    ~CentralTabWidget();

protected:
    void tabInserted(int index) override;
    void tabSelected(int index, int orgIndex) override;

    void showEvent(QShowEvent *event) override;

signals:
};

#endif // CENTRALTABWIDGET_H
