#ifndef QSCROLLABLETABBAR_H
#define QSCROLLABLETABBAR_H

#include <QAbstractButton>
#include <QFrame>
#include <QIcon>
#include <QScrollBar>
#include <QTabBar>
#include <QVariant>

#include "QMWidgetToolsGlobal.h"

class QScrollableTabWidget;
class QScrollableTabBarPrivate;

class QMWTOOLS_EXPORT QScrollableTabBar : public QFrame {
    Q_OBJECT
    Q_DECLARE_PRIVATE(QScrollableTabBar)

    Q_PROPERTY(double scrollOpacity READ scrollOpacity WRITE setScrollOpacity NOTIFY styleChanged)
public:
    explicit QScrollableTabBar(QWidget *parent = nullptr);
    ~QScrollableTabBar();

    friend class QScrollableTabWidget;

public:
    int addTab(const QString &text);
    int addTab(const QIcon &icon, const QString &text);

    int insertTab(int index, const QString &text);
    int insertTab(int index, const QIcon &icon, const QString &text);

    void removeTab(int index);
    void moveTab(int from, int to);

    QString tabText(int index) const;
    void setTabText(int index, const QString &text);

    QIcon tabIcon(int index) const;
    void setTabIcon(int index, const QIcon &icon);

    QString tabToolTip(int index) const;
    void setTabToolTip(int index, const QString &tip);

    QVariant tabData(int index) const;
    void setTabData(int index, const QVariant &data);

    QRect tabRect(int index) const;
    int tabAt(const QPoint &pos) const;
    int totalWidth() const;

    int currentIndex() const;
    int count() const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

    QSize iconSize(int index) const;
    void setIconSize(int index, const QSize &size);

    QTabBar::SelectionBehavior selectionBehaviorOnRemove() const;
    void setSelectionBehaviorOnRemove(QTabBar::SelectionBehavior behavior);

    void setCurrentIndex(int index);

signals:
    void currentChanged(int index, int orgIndex);
    void tabCloseRequested(int index);
    void tabMoved(int from, int to);
    void tabBarClicked(Qt::MouseButton button, int index);

protected:
    virtual void tabInserted(int index);
    virtual void tabRemoved(int index);

    void resizeEvent(QResizeEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    QScrollableTabBar(QScrollableTabBarPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<QScrollableTabBarPrivate> d_ptr;

private:
    void _q_scrollBarValueChanged(int value);
    void _q_scrollBarRangeChanged(int min, int max);
    void _q_closeTab();

public:
    double scrollOpacity() const;
    void setScrollOpacity(double opacity);

    bool autoHide() const;
    void setAutoHide(bool autoHide);

    QScrollBar *scrollBar() const;

    int currentDraggedIndex();
    static QScrollableTabBar *currentDraggedTabBar();

    QAbstractButton *closeButton(int index) const;
    void setCloseButton(int index, QAbstractButton *button);

signals:
    void styleChanged();
};

#endif // QSCROLLABLETABBAR_H
