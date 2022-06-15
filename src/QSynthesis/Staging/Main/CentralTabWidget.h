#ifndef CENTRALTABWIDGET_H
#define CENTRALTABWIDGET_H

#include "QScrollableTabWidget.h"

class CentralTabWidgetPrivate;

class CentralTabWidget : public QScrollableTabWidget {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CentralTabWidget)
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

protected:
    CentralTabWidget(CentralTabWidgetPrivate &d, QWidget *parent = nullptr);
};

#endif // CENTRALTABWIDGET_H
