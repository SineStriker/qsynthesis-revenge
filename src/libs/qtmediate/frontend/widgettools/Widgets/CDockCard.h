#ifndef CDOCKCARD_H
#define CDOCKCARD_H

#include "CLTabButton.h"
#include "QMNamespace.h"

#include "QMWidgetToolsGlobal.h"

class CDockTabDragProxy;

class CDockCardPrivate;

class QMWTOOLS_EXPORT CDockCard : public CLTabButton {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CDockCard)
    Q_PROPERTY(QSize dragOffset READ dragOffset WRITE setDragOffset)
public:
    explicit CDockCard(QWidget *parent = nullptr);
    explicit CDockCard(const QString &text, QWidget *parent = nullptr);
    CDockCard(const QIcon &icon, const QString &text, QWidget *parent = nullptr);
    ~CDockCard();

    enum ViewMode {
        DockPinned,
        Float,
        Window,
    };

    Q_ENUM(ViewMode)

public:
    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orient);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;
    int widthHint() const;

    QSize dragOffset() const;
    void setDragOffset(const QSize &dragOffset);

    QWidget *widget() const;
    QWidget *container() const;
    QWidget *takeWidget();
    void setWidget(QWidget *widget);

    ViewMode viewMode() const;
    void setViewMode(ViewMode viewMode);

    inline bool dockVisible() const;
    Qt::Edge edge() const;
    QM::Priority priority() const;

    void moveWidget(const QPoint &pos);
    void toggleMaximizeState();
    void setSize(const QSize &size);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void leaveEvent(QEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void viewModeChanged(ViewMode viewMode, ViewMode oldViewMode);

private:
    QScopedPointer<CDockCardPrivate> d_ptr;

    CDockCard(CDockCardPrivate &d, QWidget *parent = nullptr);

    friend class CDockTabBar;
    friend class CDockTabDragProxy;
    friend class CDockFrame;
    friend class CDockToolWindow;
    friend class CDockToolWindowPrivate;
};

inline bool CDockCard::dockVisible() const {
    return isChecked() && viewMode() == DockPinned;
}

#endif // CDOCKCARD_H
