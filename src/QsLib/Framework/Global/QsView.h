#ifndef QSVIEWHELPER_H
#define QSVIEWHELPER_H

#include <QPen>
#include <QPixmap>

#include "QsFrameworkGlobal.h"

namespace QsView {

    QSFRAMEWORK_API QPixmap createBitmapFromSVG(QString fullpath, QSize size);

    QSFRAMEWORK_API QPixmap createDeviceRenderPixmap(QWindow *refWindow, QSize logicalPixelSize);

    QSFRAMEWORK_API void drawBorderShadow(QPainter &painter, const QMargins &margin,
                                          const QSize &size, const QColor &color = Qt::black);

    QSFRAMEWORK_API void drawBorderLine(QPainter &painter, const QMargins &margin,
                                        const QSize &size, const QPen &pen = QPen(Qt::black));

    QSFRAMEWORK_API bool widgetHitTest(QWidget *w1, QWidget *w2);

    QSFRAMEWORK_API bool rectHitTest(const QRect &rect1, const QRect &rect2);

    QSFRAMEWORK_API bool rectHitTest(const QRectF &rect1, const QRectF &rect2);

    QSFRAMEWORK_API void waitToShow(QWidget *w);

    QSFRAMEWORK_API void centralizeWindow(QWidget *w, QSizeF ratio = QSizeF(-1, -1));

    QSFRAMEWORK_API void enableScaling(); // unused

    QSFRAMEWORK_API QPoint fixDesktopPos(const QPoint &pos, const QSize &size);

    QSFRAMEWORK_API void bringWindowToForeground(QWidget *w);

} // namespace View

#endif // QSVIEWHELPER_H
