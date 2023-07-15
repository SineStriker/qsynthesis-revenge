#ifndef CHORUSKIT_QMFLOATINGWINDOWHELPER_P_H
#define CHORUSKIT_QMFLOATINGWINDOWHELPER_P_H

#include <QRect>
#include <QSet>

#include "QMFloatingWindowHelper.h"

class QMFloatingWindowHelperPrivate : public QObject {
    Q_OBJECT
public:
    QMFloatingWindowHelperPrivate(QWidget *w, QMFloatingWindowHelper *q);
    ~QMFloatingWindowHelperPrivate();

    QMFloatingWindowHelper *q;

    void setFloating_helper(bool floating, Qt::WindowFlags flags);

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


    QPoint m_pressedPos;
    QRect m_orgGeometry;
    QRect m_rect;

    Qt::MouseButton m_pressedButton;
    EdgeAndCorner m_pressedArea;
    QRect m_pressedRect[SizeOfEdgeAndCorner];

    bool dummyEventFilter(QObject *obj, QEvent *event);

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

    friend class QMFloatingWindowHelper;
};

#endif // CHORUSKIT_QMFLOATINGWINDOWHELPER_P_H
