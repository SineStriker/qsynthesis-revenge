
#include "VTreeItemDelegate.h"
#include "../VSliceListWidget.h"
#include "../VSpkTreeWidget.h"

#include "../VExplorerPanel.h"

#include <QPainter>

VTreeItemDelegate::VTreeItemDelegate(QObject *parent) : QStyledItemDelegate(parent) {
    m_margins = QMargins(5, 5, 5, 5);
    m_langTagMargins = QMargins(5, 5, 5, 5);
}

VTreeItemDelegate::~VTreeItemDelegate() {
}

QSize VTreeItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    
    size.setHeight(m_lineHeight.value());

    return size;
}

void VTreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const {
    QRect bgRect = option.rect.adjusted(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());

    QRect rect = bgRect.adjusted(m_contentMargins.left(), m_contentMargins.top(), -m_contentMargins.right(), -m_contentMargins.bottom());

    // Fetch data
    QString langText = index.data(VExplorerPanel::LVItem_LangName).toString();

    // Calculate size
    QFont itemTextFont = m_itemTextType.font();

    QFontMetrics itemTextMetrics(itemTextFont);

    // Only calculate language tag size when the item is assigned with a language short name
    QRect langTagRect;
    QFont langTextFont = m_langTextType.font();
    int langTagWidth = 0, langTagHeight = 0;
    if(!langText.isEmpty()) {
        QFontMetrics langTextMetrics(langTextFont);
        int langTextWidth = langTextMetrics.horizontalAdvance(langText);
        langTagWidth = langTextWidth + m_langTagMargins.left() + m_langTagMargins.right();
        langTagHeight = m_langTagHeight.value();

        langTagRect.setTop(rect.top() + (rect.height() - langTagHeight) / 2.0); // Center with `rect`
        langTagRect.setLeft(rect.right() - langTagWidth); // Right aligned
        langTagRect.setWidth(langTagWidth);
        langTagRect.setHeight(langTagHeight);
    }

    QRect textRect;
    textRect.setTop(rect.top());
    textRect.setLeft(rect.left());
    textRect.setWidth(rect.width() - langTagWidth - m_langTagMargins.left());
    textRect.setHeight(rect.height());
    QString text = index.data().toString();
    text = itemTextMetrics.elidedText(text, Qt::ElideRight, textRect.width());

    painter->setRenderHint(QPainter::Antialiasing);

    // Draw background for selected or expanded
    if (option.state & (QStyle::State_Selected)) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_colors[0]);
        painter->drawRoundedRect(bgRect, m_lineRoundEdge.valueF(), m_lineRoundEdge.valueF());
    } 

    // Language Tag
    if(!langText.isEmpty()) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(index.data(VExplorerPanel::LVItem_LangColor).value<QColor>());
        painter->drawRoundedRect(langTagRect, langTagHeight / 2.0, langTagHeight / 2.0);

        painter->setPen(m_langTextType.color());
        painter->setFont(langTextFont);
        painter->drawText(langTagRect, Qt::AlignCenter, langText);
    }
    
    // Text
    painter->setPen(m_itemTextType.color());
    painter->setBrush(Qt::NoBrush);
    painter->setFont(m_itemTextType.font());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
}
