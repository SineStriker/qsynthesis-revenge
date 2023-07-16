#ifndef CHORUSKIT_ICONCOLORIMPL_H
#define CHORUSKIT_ICONCOLORIMPL_H

#include <QPen>
#include <QPushButton>
#include <QToolButton>

#include <QMNamespace.h>

namespace IconColorImpl {

    void correctIconStateAndColor(QIcon &icon, QM::ClickState state, const QString &salt,
                                  const std::function<QString()> &getColor);

    void getTextColor(QPen &pen, const QSize &size, const std::function<void(QPainter *)> &paint);

    void getTextColor(QPen &pen, QPointF &pos, const QSize &size, const std::function<void(QPainter *)> &paint);

    QM::ClickState getButtonClickState(QAbstractButton *button);

};

#endif // CHORUSKIT_ICONCOLORIMPL_H
