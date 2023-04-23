#include "FullIconButton.h"

#include <QDebug>
#include <QStyleOptionButton>
#include <QStylePainter>

namespace QsApi {

    FullIconButton::FullIconButton(QWidget *parent) : CPushButton(parent) {
    }

    FullIconButton::~FullIconButton() {
    }

    void FullIconButton::paintEvent(QPaintEvent *event) {
        Q_UNUSED(event)

        QStylePainter p(this);
        QStyleOptionButton option;
        initStyleOption(&option);

        QSize iconSz = iconSize();
        QSize sz = contentsRect().size() - 2 * m_iconMargin;

        double iconAspect = double(iconSz.width()) / iconSz.height();
        double aspect = double(width()) / height();
        if (iconAspect > aspect) {
            sz.setHeight(sz.width() / iconAspect);
        } else {
            sz.setWidth(sz.height() * iconAspect);
        }

        option.icon = icon();
        option.iconSize = sz;

        p.drawControl(QStyle::CE_PushButton, option); // From Qt source
    }

}