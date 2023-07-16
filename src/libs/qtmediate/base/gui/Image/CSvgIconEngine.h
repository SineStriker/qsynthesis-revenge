#ifndef CSVGICONENGINE_H
#define CSVGICONENGINE_H

#include <QIconEngine>

#include "QMGuiGlobal.h"
#include "QMNamespace.h"

class CSvgIconEnginePrivate;

class QMGUI_EXPORT CSvgIconEngine : public QIconEngine {
public:
    CSvgIconEngine();
    CSvgIconEngine(const CSvgIconEngine &other);
    ~CSvgIconEngine();

    void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state) override;
    QSize actualSize(const QSize &size, QIcon::Mode mode, QIcon::State state) override;
    QPixmap pixmap(const QSize &size, QIcon::Mode mode, QIcon::State state) override;

    void addFile(const QString &fileName, const QSize &size, QIcon::Mode mode, QIcon::State state) override;

    QString key() const override;
    QIconEngine *clone() const override;
    bool read(QDataStream &in) override;
    bool write(QDataStream &out) const override;

    void virtual_hook(int id, void *data) override;

public:
    QM::ClickState currentState() const;
    void setCurrentState(QM::ClickState state);

    QString salt() const;
    void setSalt(const QString &salt);

    bool needColorHint() const;
    void setColorHint(const QString &color);

    void setValues(QByteArray *dataList, QString *colorList);

private:
    QScopedPointer<CSvgIconEnginePrivate> d;
};

#endif // CSVGICONENGINE_H
