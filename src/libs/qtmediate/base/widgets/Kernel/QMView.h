#ifndef QMVIEW_H
#define QMVIEW_H

#include <QPen>
#include <QPixmap>

#include "QMWidgetsGlobal.h"

namespace QMView {

    QMWIDGETS_EXPORT QPixmap createBitmapFromSVG(QString fullpath, QSize size);

    QMWIDGETS_EXPORT QPixmap createDeviceRenderPixmap(QWindow *refWindow, QSize logicalPixelSize);

    QMWIDGETS_EXPORT void drawBorderShadow(QPainter &painter, const QMargins &margin,
                                          const QSize &size, const QColor &color = Qt::black);

    QMWIDGETS_EXPORT void drawBorderLine(QPainter &painter, const QMargins &margin,
                                        const QSize &size, const QPen &pen = QPen(Qt::black));

    QMWIDGETS_EXPORT bool widgetHitTest(QWidget *w1, QWidget *w2);

    QMWIDGETS_EXPORT bool rectHitTest(const QRect &rect1, const QRect &rect2);

    QMWIDGETS_EXPORT bool rectHitTest(const QRectF &rect1, const QRectF &rect2);

    QMWIDGETS_EXPORT void waitToShow(QWidget *w);

    QMWIDGETS_EXPORT void centralizeWindow(QWidget *w, QSizeF ratio = QSizeF(-1, -1));

    QMWIDGETS_EXPORT QPoint fixDesktopPos(const QPoint &pos, const QSize &size);

    QMWIDGETS_EXPORT void bringWindowToForeground(QWidget *w);

    QMWIDGETS_EXPORT QWidget *implicitMouseGrabber();

}

#endif // QMVIEW_H
