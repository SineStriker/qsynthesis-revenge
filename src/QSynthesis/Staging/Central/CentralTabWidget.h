#ifndef CENTRALTABWIDGET_H
#define CENTRALTABWIDGET_H

#include "QScrollableTabWidget.h"

class CentralTabWidget : public QScrollableTabWidget {
    Q_OBJECT
public:
    explicit CentralTabWidget(QWidget *parent = nullptr);
    ~CentralTabWidget();

    friend class TabManager;

public:
    void invalidateHover() const;

protected:
    void tabInserted(int index) override;
    void tabSelected(int index, int orgIndex) override;

    void showEvent(QShowEvent *event) override;

private:
    void _q_tabTitleChanged(const QString &title);
    void _q_tabIconChanged(const QIcon &icon);

signals:
    void currentTabTextChanged(const QString &text);
};

#endif // CENTRALTABWIDGET_H
