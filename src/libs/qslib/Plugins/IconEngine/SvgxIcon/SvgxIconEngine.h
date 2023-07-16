#ifndef SVGXICONENGINE_H
#define SVGXICONENGINE_H

#include <QIconEngine>

#include "QMNamespace.h"

class SvgxIconEnginePrivate;

class SvgxIconEngine : public QIconEngine {
public:
    SvgxIconEngine();
    SvgxIconEngine(const SvgxIconEngine &other);
    ~SvgxIconEngine();

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

    QString stateColor(QM::ClickState state) const;
    void setStateColor(QM::ClickState state, const QString &color);

    QString salt() const;
    void setSalt(const QString &salt);

    void setValues(QByteArray *dataList, QString *colorList);

private:
    QScopedPointer<SvgxIconEnginePrivate> d;
};


#endif // SVGXICONENGINE_H
