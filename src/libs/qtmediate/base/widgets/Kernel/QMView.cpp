#include "QMView.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QPainter>
#include <QScreen>
#include <QSvgRenderer>
#include <QWidget>
#include <QWindow>

#ifdef Q_OS_WINDOWS
#include <Windows.h>
#endif

QPixmap QMView::createBitmapFromSVG(QString fullpath, QSize size) {
    QSvgRenderer svgRender(fullpath);
    QPixmap bmp(size);
    bmp.fill(Qt::transparent);
    QPainter painter(&bmp);
    painter.setRenderHint(QPainter::Antialiasing);
    svgRender.render(&painter);
    return bmp;
}

/**
 * @brief Create a pixmap whose resolution and pixel ratio is suitable for rendering to screen.
 * @param refWindow pointer to QWindow whose DPR is taken. If it's nullptr, use qApp's DPR instead
 * @param logicalPixelSize Desired size of pixmap, in OS logical pixels
 * @return created pixmap object
 */
QPixmap QMView::createDeviceRenderPixmap(QWindow *refWindow, QSize logicalPixelSize) {
#ifdef Q_OS_MACOS
    qreal targetDPR;
    if (refWindow)
        targetDPR = refWindow->devicePixelRatio();
    else
        targetDPR = qApp->devicePixelRatio();
    QPixmap ret(logicalPixelSize * targetDPR);
    ret.setDevicePixelRatio(targetDPR);
    return ret;
#else
    Q_UNUSED(refWindow);
    return QPixmap(logicalPixelSize);
#endif
}

void QMView::drawBorderShadow(QPainter &painter, const QMargins &margin, const QSize &size,
                              const QColor &color) {
    int w = size.width();
    int h = size.height();

    int cw = w - margin.left() - margin.right();
    int ch = h - margin.top() - margin.bottom();

    painter.setPen(Qt::NoPen);

    // Left
    QRect r1(0, margin.top(), margin.left(), ch);
    QLinearGradient l1(r1.left(), 0, r1.right(), 0);
    l1.setColorAt(0, Qt::transparent);
    l1.setColorAt(1, color);

    painter.setBrush(QBrush(l1));
    painter.drawRect(r1);

    // Top
    QRect r2(margin.left(), 0, cw, margin.top());
    QLinearGradient l2(0, r2.top(), 0, r2.bottom());
    l2.setColorAt(0, Qt::transparent);
    l2.setColorAt(1, color);

    painter.setBrush(QBrush(l2));
    painter.drawRect(r2);

    // Right
    QRect r3(w - margin.right(), margin.top(), margin.right(), ch);
    QLinearGradient l3(r3.left(), 0, r3.right(), 0);
    l3.setColorAt(0, color);
    l3.setColorAt(1, Qt::transparent);

    painter.setBrush(QBrush(l3));
    painter.drawRect(r3);

    // Bottom
    QRect r4(margin.left(), h - margin.bottom(), cw, margin.bottom());
    QLinearGradient l4(0, r4.top(), 0, r4.bottom());
    l4.setColorAt(0, color);
    l4.setColorAt(1, Qt::transparent);

    painter.setBrush(QBrush(l4));
    painter.drawRect(r4);

    int maxRadius = qMax(qMax(margin.left(), margin.right()), qMax(margin.top(), margin.bottom()));
    QPixmap bmp(QSize(maxRadius * 2, maxRadius * 2));
    bmp.fill(Qt::transparent);

    QPainter tempPainter(&bmp);
    QRadialGradient rg(maxRadius, maxRadius, maxRadius, maxRadius, maxRadius);
    rg.setColorAt(0, color);
    rg.setColorAt(1, Qt::transparent);

    tempPainter.setPen(Qt::NoPen);
    tempPainter.setBrush(QBrush(rg));
    tempPainter.drawEllipse(0, 0, 2 * maxRadius, 2 * maxRadius);

    QPixmap topLeftBmp = bmp.copy(0, 0, maxRadius, maxRadius);
    QPixmap topRightBmp = bmp.copy(maxRadius, 0, maxRadius, maxRadius);
    QPixmap bottomLeftBmp = bmp.copy(0, maxRadius, maxRadius, maxRadius);
    QPixmap bottomRightBmp = bmp.copy(maxRadius, maxRadius, maxRadius, maxRadius);

    // Top Left
    painter.drawPixmap(0, 0, margin.left(), margin.top(),
                       topLeftBmp.scaled(margin.left(), margin.top()));
    // Top Right
    painter.drawPixmap(w - margin.right(), 0, margin.right(), margin.top(),
                       topRightBmp.scaled(margin.right(), margin.top()));
    // Bottom Left
    painter.drawPixmap(0, h - margin.bottom(), margin.left(), margin.top(),
                       bottomLeftBmp.scaled(margin.left(), margin.bottom()));
    // Bottom Right
    painter.drawPixmap(w - margin.right(), h - margin.bottom(), margin.right(), margin.bottom(),
                       bottomRightBmp.scaled(margin.right(), margin.bottom()));
}

void QMView::drawBorderLine(QPainter &painter, const QMargins &margin, const QSize &size,
                            const QPen &pen) {
    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    QRectF rect;
    rect.setLeft(margin.left() - pen.widthF());
    rect.setRight(size.width() - margin.right() + pen.widthF());
    rect.setTop(margin.top() - pen.widthF());
    rect.setBottom(size.height() - margin.bottom() + pen.widthF());
    painter.drawRect(rect);
}

bool QMView::widgetHitTest(QWidget *w1, QWidget *w2) {
    QRect rect1 = QRect(w1->mapToGlobal(QPoint(0, 0)), w1->size());
    QRect rect2 = QRect(w2->mapToGlobal(QPoint(0, 0)), w2->size());
    return rectHitTest(rect1, rect2);
}

bool QMView::rectHitTest(const QRect &rect1, const QRect &rect2) {
    return rectHitTest(QRectF(rect1), QRectF(rect2));
}

bool QMView::rectHitTest(const QRectF &rect1, const QRectF &rect2) {
    if (rect1.x() + rect1.width() > rect2.x() && rect2.x() + rect2.width() > rect1.x() &&
        rect1.y() + rect1.height() > rect2.y() && rect2.y() + rect2.height() > rect1.y()) {
        return true;
    } else {
        return false;
    }
}

void QMView::waitToShow(QWidget *w) {
    while (!w->isVisible()) {
        qApp->processEvents(QEventLoop::AllEvents);
    }
}

void QMView::centralizeWindow(QWidget *w, QSizeF ratio) {
    QSize desktopSize;
    if (w->parentWidget()) {
        desktopSize = w->parentWidget()->size();
    } else {
        desktopSize = w->screen()->size();
    }

    int dw = desktopSize.width();
    int dh = desktopSize.height();

    double rw = ratio.width();
    double rh = ratio.height();

    QSize size = w->size();
    if (rw > 0 && rw <= 1) {
        size.setWidth(dw * rw);
    }
    if (rh > 0 && rh <= 1) {
        size.setHeight(dh * rh);
    }

    w->setGeometry((dw - size.width()) / 2, (dh - size.height()) / 2, size.width(), size.height());
}

QPoint QMView::fixDesktopPos(const QPoint &pos, const QSize &size) {
    QPoint target(pos);
    if (target.x() < 0) {
        target.setX(0);
    }
    if (target.y() < 0) {
        target.setY(0);
    }
    int dw = qApp->desktop()->width();
    int dh = qApp->desktop()->height();
    int w = size.width();
    int h = size.height();
    if (target.x() + w > dw) {
        target.setX(dw - w);
    }
    if (target.y() + h > dh) {
        target.setY(dh - h);
    }
    return target;
}

void QMView::bringWindowToForeground(QWidget *w) {
    // Make sure the window isn't minimized
    // TODO: this always puts it in the "normal" state but it might have been maximized
    // before minimized...so either a flag needs stored or find a Qt call to do it appropriately
    if (w->isMinimized())
        w->showNormal();

#ifdef Q_OS_WIN
    // TODO: there doesn't seem to be a cross platform way to force the window
    // to the foreground. So this will need moved to a platform specific file

    HWND hWnd = reinterpret_cast<HWND>(w->effectiveWinId());
    if (hWnd) {
        // I have no idea what this does but it works mostly
        // https://www.codeproject.com/Articles/1724/Some-handy-dialog-box-tricks-tips-and-workarounds

        ::AttachThreadInput(::GetWindowThreadProcessId(::GetForegroundWindow(), nullptr),
                            ::GetCurrentThreadId(), TRUE);

        ::SetForegroundWindow(hWnd);
        ::SetFocus(hWnd);

        ::AttachThreadInput(GetWindowThreadProcessId(GetForegroundWindow(), nullptr),
                            GetCurrentThreadId(), FALSE);
    }
#endif
}

extern Q_DECL_IMPORT QWidget *qt_button_down;

QWidget *QMView::implicitMouseGrabber() {
    return qt_button_down;
}