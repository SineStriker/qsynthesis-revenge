
#pragma once

#include <QGraphicsRectItem>

class StretchingPixmapItem : public QGraphicsRectItem
{
public:
    StretchingPixmapItem(QGraphicsItem *parent = nullptr);
    ~StretchingPixmapItem();

    void SetPixmap(const QPixmap &pixmap);
    void SetPixmap(const QImage &image);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

private:
    QPixmap mPixmap;
};
