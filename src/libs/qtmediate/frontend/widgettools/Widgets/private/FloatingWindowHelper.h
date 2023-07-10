#ifndef FLOATINGWINDOWHELPER_H
#define FLOATINGWINDOWHELPER_H

#include <QObject>
#include <QRect>
#include <QSet>

class FloatingWindowHelper : public QObject {
    Q_OBJECT
public:
    explicit FloatingWindowHelper(QWidget *w, QObject *parent = nullptr);
    ~FloatingWindowHelper();

public:
    void addTitleBarWidget(QWidget *w);
    void removeTitleBarWidget(QWidget *w);

    bool floating() const;
    void setFloating(bool floating, Qt::WindowFlags flags = Qt::Window);

    QMargins resizeMargins() const;
    void setResizeMargins(const QMargins &resizeMargins);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

    enum EdgeAndCorner {
        None = -1,
        Left = 0,
        Right,
        Top,
        Bottom,
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        SizeOfEdgeAndCorner,
    };

    QWidget *w;
    QMargins m_resizeMargins;

    bool m_floating;
    int m_windowFlags;

    bool m_pressed;
    QPoint m_pressedPos;
    QRect m_orgGeometry;

    EdgeAndCorner m_pressedArea;
    QRect m_pressedRect[SizeOfEdgeAndCorner];

    QSet<QWidget *> m_titleBarWidgets;

    bool dummyEventFilter(QObject *obj, QEvent *event);

private:
    void _q_titleBarWidgetDestroyed();
};

#endif // FLOATINGWINDOWHELPER_H
