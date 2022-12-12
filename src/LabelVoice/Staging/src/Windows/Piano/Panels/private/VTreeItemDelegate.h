#ifndef VTREEITEMDELEGATE_H
#define VTREEITEMDELEGATE_H

#include <QStyledItemDelegate>

#include "QTypeFace.h"
#include "QPixelSize.h"
#include "QColorList.h"

/**
    @brief Used in tree items of Explorer panel, for customized painting, and handling the
    trailing language labels
*/
class VTreeItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit VTreeItemDelegate(QObject *parent = nullptr);
    ~VTreeItemDelegate();

    friend class VSpkTreeWidget;

public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

protected:
    QPixelSize m_lineHeight;
    QPixelSize m_lineRoundEdge;
    QPixelSize m_langTagHeight;

    QTypeFace m_itemTextType;
    QTypeFace m_langTextType;

    QMargins m_langTagMargins;
    QMargins m_contentMargins;
    QMargins m_margins;

    QColorList m_colors;
    /* 0: selectedColor (for selected/expanded item background)
     */

};

#endif // VTREEITEMDELEGATE_H
