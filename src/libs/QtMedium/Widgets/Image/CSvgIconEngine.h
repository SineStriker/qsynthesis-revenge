#ifndef CSVGICONENGINE_H
#define CSVGICONENGINE_H

#include <QIconEngine>

#include "QMWidgetsGlobal.h"

class CSvgIconEnginePrivate;

class CSvgIconEngine : public QIconEngine {
public:
    CSvgIconEngine();
    CSvgIconEngine(const CSvgIconEngine &other);
    CSvgIconEngine(const QString &currentColor);
    ~CSvgIconEngine();

    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
    QSize actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;

    void addFile(const QString &fileName, const QSize &size, QIcon::Mode mode,
                 QIcon::State state) override;

    QString key() const override;
    QIconEngine *clone() const override;
    bool read(QDataStream &in) override;
    bool write(QDataStream &out) const override;

    void virtual_hook(int id, void *data) override;

public:
    QString currentColor() const;
    void setCurrentColor(const QString &currentColor);

private:
    QSharedDataPointer<CSvgIconEnginePrivate> d;
};

#endif // CSVGICONENGINE_H
