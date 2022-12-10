
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

    int langBoxHeight = m_langTagHeight.value() + m_langTagMargins.top() + m_langTagMargins.bottom();
    int h = langBoxHeight + m_margins.top() + m_margins.bottom();
    size.setHeight(h);

    return size;
}

void VTreeItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const {
    QRect origRect = option.rect;

    // painter->setPen(Qt::red);
    // painter->drawRect(rect);
    QRect rect = origRect.adjusted(m_margins.left(), m_margins.top(), -m_margins.right(), -m_margins.bottom());
    // painter->setPen(Qt::green);
    // painter->drawRect(rect);

    // Fetch data
    QString langText = index.data(VExplorerPanel::LVItem_LangName).toString();

    // Calculate size
    QFont langTextFont = m_langTextType.font();
    QFont itemTextFont = m_itemTextType.font();

    QFontMetrics langTextMetrics(langTextFont);
    QFontMetrics itemTextMetrics(itemTextFont);

    int langTextWidth = langTextMetrics.horizontalAdvance(langText);
    int langTagWidth = langTextWidth + m_langTagMargins.left() + m_langTagMargins.right();
    int langTagHeight = m_langTagHeight.value();

    QRect langTagRect;
    langTagRect.setTop(rect.top() + (rect.height() - langTagHeight) / 2.0); // Center with `rect`
    langTagRect.setLeft(rect.right() - langTagWidth); // Right aligned
    langTagRect.setWidth(langTagWidth);
    langTagRect.setHeight(langTagHeight);

    QRect textRect;
    textRect.setTop(rect.top());
    textRect.setLeft(rect.left());
    textRect.setWidth(rect.width() - langTagWidth - m_langTagMargins.left());
    textRect.setHeight(rect.height());
    QString text = index.data().toString();
    text = itemTextMetrics.elidedText(text, Qt::ElideRight, textRect.width());

    painter->setRenderHint(QPainter::Antialiasing);

    // Draw background for selected or expanded
    if (option.state & (QStyle::State_Selected | QStyle::State_Open)) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(m_colors[0]);
        painter->drawRoundedRect(origRect, 3, 3); // FIXME: use the custom style system
    } 

    // Language Tag
    painter->setBrush(index.data(VExplorerPanel::LVItem_LangColor).value<QColor>());
    painter->drawRoundedRect(langTagRect, langTagHeight / 2.0, langTagHeight / 2.0);

    painter->setPen(m_langTextType.color());
    painter->setFont(langTextFont);
    painter->drawText(langTagRect, Qt::AlignCenter, langText);
    
    // Text
    painter->setPen(m_itemTextType.color());
    painter->setFont(m_itemTextType.font());
    painter->drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, text);
}
