#ifndef QMViewHELPER_H
#define QMViewHELPER_H

#include <QPen>
#include <QPixmap>

#include "QMWidgetsGlobal.h"

namespace QMView {

    QMWIDGETS_API QPixmap createBitmapFromSVG(QString fullpath, QSize size);

    QMWIDGETS_API QPixmap createDeviceRenderPixmap(QWindow *refWindow, QSize logicalPixelSize);

    QMWIDGETS_API void drawBorderShadow(QPainter &painter, const QMargins &margin,
                                          const QSize &size, const QColor &color = Qt::black);

    QMWIDGETS_API void drawBorderLine(QPainter &painter, const QMargins &margin,
                                        const QSize &size, const QPen &pen = QPen(Qt::black));

    QMWIDGETS_API bool widgetHitTest(QWidget *w1, QWidget *w2);

    QMWIDGETS_API bool rectHitTest(const QRect &rect1, const QRect &rect2);

    QMWIDGETS_API bool rectHitTest(const QRectF &rect1, const QRectF &rect2);

    QMWIDGETS_API void waitToShow(QWidget *w);

    QMWIDGETS_API void centralizeWindow(QWidget *w, QSizeF ratio = QSizeF(-1, -1));

    QMWIDGETS_API void enableScaling(); // unused

    QMWIDGETS_API QPoint fixDesktopPos(const QPoint &pos, const QSize &size);

    QMWIDGETS_API void bringWindowToForeground(QWidget *w);

    QMWIDGETS_API QWidget *implicitMouseGrabber();

}

#endif // QMViewHELPER_H
