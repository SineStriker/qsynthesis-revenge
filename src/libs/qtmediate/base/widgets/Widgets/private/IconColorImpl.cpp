#include "IconColorImpl.h"

#include <QImage>

#include <private/qicon_p.h>
#include <private/qimage_p.h>
#include <private/qpaintengine_p.h>
#include <private/qpaintengine_raster_p.h>
#include <private/qpixmap_raster_p.h>

#include <QMSvg.h>
#include <private/QMGuiAppExtension_p.h>

class HackPaintEngine : public QRasterPaintEngine {
public:
    HackPaintEngine(QPaintDevice *device) : QRasterPaintEngine(device) {
    }
    ~HackPaintEngine() = default;

    void fillRect(const QRectF &rect, const QBrush &brush) override{};
    void fillRect(const QRectF &rect, const QColor &color) override{};

    void drawRects(const QRect *rects, int rectCount) override{};
    void drawRects(const QRectF *rects, int rectCount) override{};

    void drawLines(const QLine *lines, int lineCount) override{};
    void drawLines(const QLineF *lines, int lineCount) override{};

    void drawEllipse(const QRectF &r) override{};
    void drawEllipse(const QRect &r) override{};

    void drawPath(const QPainterPath &path) override{};

    void drawPoints(const QPointF *points, int pointCount) override{};
    void drawPoints(const QPoint *points, int pointCount) override{};

    void drawPolygon(const QPointF *points, int pointCount, PolygonDrawMode mode) override{};
    void drawPolygon(const QPoint *points, int pointCount, PolygonDrawMode mode) override{};

    void drawPixmap(const QRectF &r, const QPixmap &pm, const QRectF &sr) override{};
    void drawTextItem(const QPointF &p, const QTextItem &textItem) override {
        pen = painter()->pen();
        pos = p;
    }
    void drawTiledPixmap(const QRectF &r, const QPixmap &pixmap, const QPointF &s) override{};
    void drawImage(const QRectF &r, const QImage &pm, const QRectF &sr,
                   Qt::ImageConversionFlags flags = Qt::AutoColor) override{};

    QPen pen;
    QPointF pos;
};

namespace IconColorImpl {

    void correctIconStateAndColor(QIcon &icon, QM::ClickState state, const QString &salt,
                                  const std::function<QString()> &getColor) {
        Q_UNUSED(salt);

        if (!QMSvg::update(&icon, state, salt))
            return;

        if (QMSvg::getColor(&icon, state) == "auto") {
            QMSvg::setColor(&icon, state, getColor());
        }
    }

    void getTextColor(QPen &pen, const QSize &size, const std::function<void(QPainter *)> &paint) {
        QImage image(size, QImage::Format_ARGB32);

        auto engine = new HackPaintEngine(&image);
        image.data_ptr()->paintEngine = engine;

        QPainter p2(&image);
        paint(&p2);

        pen = engine->pen;
    }

    QString defaultSalt(QObject *obj) {
        return QString("%1_%2_%3_%4")
            .arg(obj->metaObject()->className(), obj->objectName(), QString::number(quintptr(obj), 16),
                 QString::number(QMGuiAppExtensionPrivate::globalImageCacheSerialNum, 16));
    }

    QM::ClickState getButtonClickState(QAbstractButton *button) {
        if (button->isChecked()) {
            if (!button->isEnabled()) {
                return QM::CS_Disabled_Checked;
            }
            if (button->isDown()) {
                return QM::CS_Pressed_Checked;
            }
            if (button->underMouse()) {
                return QM::CS_Hover_Checked;
            }
            return QM::CS_Normal_Checked;
        }

        if (!button->isEnabled()) {
            return QM::CS_Disabled;
        }
        if (button->isDown()) {
            return QM::CS_Pressed;
        }
        if (button->underMouse()) {
            return QM::CS_Hover;
        }
        return QM::CS_Normal;
    }

}
