#ifndef VIEWHELPER_H
#define VIEWHELPER_H

#include <QPen>
#include <QPixmap>

namespace View {

    QPixmap createBitmapFromSVG(QString fullpath, QSize size);

    void drawBorderShadow(QPainter &painter, const QMargins &margin, const QSize &size,
                          const QColor &color = Qt::black);

    void drawBorderLine(QPainter &painter, const QMargins &margin, const QSize &size,
                        const QPen &pen = QPen(Qt::black));

    bool widgetHitTest(QWidget *w1, QWidget *w2);

    bool rectHitTest(const QRect &rect1, const QRect &rect2);

    void waitToShow(QWidget *w);

} // namespace View

#endif // VIEWHELPER_H
