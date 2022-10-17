#ifndef VIEWHELPER_H
#define VIEWHELPER_H

#include <QPen>
#include <QPixmap>

#include "qsframework_global.h"

namespace View {

    QSFRAMEWORK_API QPixmap createBitmapFromSVG(QString fullpath, QSize size);

    QSFRAMEWORK_API void drawBorderShadow(QPainter &painter, const QMargins &margin,
                                          const QSize &size, const QColor &color = Qt::black);

    QSFRAMEWORK_API void drawBorderLine(QPainter &painter, const QMargins &margin,
                                        const QSize &size, const QPen &pen = QPen(Qt::black));

    QSFRAMEWORK_API bool widgetHitTest(QWidget *w1, QWidget *w2);

    QSFRAMEWORK_API bool rectHitTest(const QRect &rect1, const QRect &rect2);

    QSFRAMEWORK_API bool rectHitTest(const QRectF &rect1, const QRectF &rect2);

    QSFRAMEWORK_API void waitToShow(QWidget *w);

    QSFRAMEWORK_API void centralizeWindow(QWidget *w, QSizeF ratio = QSizeF(-1, -1));

    QSFRAMEWORK_API QStringList extractFunctionToStringList(const QString &str, bool *ok = nullptr);

    QSFRAMEWORK_API QColor CssStringToColor(const QString &str);

    QSFRAMEWORK_API void enableScaling(); // unused

    QSFRAMEWORK_API QPoint fixDesktopPos(const QPoint &pos, const QSize &size);

} // namespace View

#endif // VIEWHELPER_H
