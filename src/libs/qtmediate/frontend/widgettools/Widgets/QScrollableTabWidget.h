#ifndef QSCROLLABLETABWIDGET_H
#define QSCROLLABLETABWIDGET_H

#include <QAbstractButton>
#include <QFrame>
#include <QMimeData>

#include "QScrollableTabBar.h"

class QScrollableTabWidgetPrivate;

class QMWTOOLS_EXPORT QScrollableTabWidget : public QFrame {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QScrollableTabWidget)
public:
    explicit QScrollableTabWidget(QWidget *parent = nullptr);
    ~QScrollableTabWidget();

public:
    QScrollableTabBar *tabBar() const;

    int addTab(QWidget *widget, const QString &label);
    int addTab(QWidget *widget, const QIcon &icon, const QString &label);

    int insertTab(int index, QWidget *widget, const QString &label);
    int insertTab(int index, QWidget *widget, const QIcon &icon, const QString &label);

    void removeTab(int index);

    QString tabText(int index) const;
    void setTabText(int index, const QString &text);

    QIcon tabIcon(int index) const;
    void setTabIcon(int index, const QIcon &icon);

    QString tabToolTip(int index) const;
    void setTabToolTip(int index, const QString &tip);

    QVariant tabData(int index) const;
    void setTabData(int index, const QVariant &data);

    int currentIndex() const;
    void setCurrentIndex(int index);

    QWidget *currentWidget() const;
    void setCurrentWidget(QWidget *widget);

    QWidget *widget(int index) const;
    int indexOf(QWidget *widget) const;
    int count() const;

    QSize iconSize(int index) const;
    void setIconSize(int index, const QSize &size);

    void clear();

signals:
    void currentChanged(int index, int orgIndex);
    void tabCloseRequested(int index);
    void tabBarClicked(Qt::MouseButton button, int index);
    void tabBarDoubleClicked(int index);

protected:
    virtual void tabInserted(int index);
    virtual void tabRemoved(int index);

    virtual void tabSelected(int index, int orgIndex);
    virtual bool tabIncoming(const QMimeData *mime) const;

    void setTabBar(QScrollableTabBar *tabBar);

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dropEvent(QDropEvent *event) override;

protected:
    QScrollableTabWidget(QScrollableTabWidgetPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<QScrollableTabWidgetPrivate> d_ptr;

public:
    QAbstractButton *closeButton(int index) const;
    void setCloseButton(int index, QAbstractButton *button);

    QWidget *tabBarWidget();
    QWidget *moveOutTabBarWidget();
    void moveInTabBarWidget();

private:
    void _q_showTab(int index, int orgIndex);
    void _q_removeTab(int index);
    void _q_tabMoved(int from, int to);

signals:
};

#endif // QSCROLLABLETABWIDGET_H
