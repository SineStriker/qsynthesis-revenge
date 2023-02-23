#ifndef BASECONTAINER_H
#define BASECONTAINER_H

#include <QFrame>
#include <QWidget>

#include "QsIntegrateGlobal.h"

class QSINTEGRATE_API BaseContainer : public QFrame {
    Q_OBJECT

    Q_PROPERTY(QMargins margins READ margins WRITE setMargins NOTIFY marginsChanged)
    Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor NOTIFY colorChanged)
    Q_PROPERTY(QColor wallpaper READ wallpaper WRITE setWallpaper NOTIFY colorChanged)

public:
    explicit BaseContainer(QWidget *parent = nullptr);
    ~BaseContainer();

public:
    QMargins margins() const;
    void setMargins(const QMargins &margins);

    QColor borderColor() const;
    void setBorderColor(const QColor &borderColor);

    QColor wallpaper() const;
    void setWallpaper(const QColor &wallpaper);

private:
    QMargins m_margins;
    QColor m_borderColor;
    QColor m_wallpaper;

    void paintEvent(QPaintEvent *event) override;

signals:
    void marginsChanged();
    void colorChanged();
};

#endif // BASECONTAINER_H
