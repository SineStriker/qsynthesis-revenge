#include "CEqualBoxLayout.h"

#include <QDebug>
#include <QSet>
#include <QWidget>

#include <private/qlayout_p.h>

class CLayoutEqualizerItem;

// EqualizerPrivate
class CEqualBoxLayoutPrivate {
public:
    CEqualBoxLayoutPrivate(CEqualBoxLayout *q) : q_ptr(q){};

    QSet<CLayoutEqualizerItem *> items;

    CEqualBoxLayout *q_ptr;
};

// Item
class CLayoutEqualizerItem : public QWidgetItemV2 {
public:
    explicit CLayoutEqualizerItem(QWidget *widget);
    ~CLayoutEqualizerItem();

protected:
    QSize sizeHint() const override;

    QSize realSizeHint() const;

    CEqualBoxLayoutPrivate *le;

    friend class CEqualBoxLayout;
};

CLayoutEqualizerItem::CLayoutEqualizerItem(QWidget *widget) : QWidgetItemV2(widget) {
}

CLayoutEqualizerItem::~CLayoutEqualizerItem() {
}

QSize CLayoutEqualizerItem::sizeHint() const {
    if (!le) {
        return realSizeHint();
    }

    QSize size(0, 0);
    for (const auto &item : qAsConst(le->items)) {
        auto real = item->realSizeHint();
        size = QSize(qMax(real.width(), size.width()), qMax(real.height(), size.height()));
    }
    return size;
}

QSize CLayoutEqualizerItem::realSizeHint() const {
    return QWidgetItemV2::sizeHint();
}

// Equalizer
CEqualBoxLayout::CEqualBoxLayout(Direction direction, QWidget *parent)
    : QBoxLayout(direction, parent), d_ptr(new CEqualBoxLayoutPrivate(this)) {
}

CEqualBoxLayout::~CEqualBoxLayout() {
}

void CEqualBoxLayout::addWidgetE(QWidget *widget, int stretch, Qt::Alignment alignment) {
    insertWidgetE(-1, widget, stretch, alignment);
}

void CEqualBoxLayout::insertWidgetE(int index, QWidget *widget, int stretch,
                                   Qt::Alignment alignment) {
    auto org = QLayoutPrivate::widgetItemFactoryMethod;

    // Exchange method temporarily
    QLayoutPrivate::widgetItemFactoryMethod = [](const QLayout *layout,
                                                 QWidget *widget) -> QWidgetItem * {
        auto item = new CLayoutEqualizerItem(widget);
        item->le = reinterpret_cast<const CEqualBoxLayout *>(layout)->d_ptr.data();
        item->le->items.insert(item);
        return item;
    };

    QBoxLayout::insertWidget(index, widget, stretch, alignment);

    QLayoutPrivate::widgetItemFactoryMethod = org;
}

void CEqualBoxLayout::addItem(QLayoutItem *item) {
    Q_D(CEqualBoxLayout);
    if (typeid(*item) == typeid(CLayoutEqualizerItem)) {
        auto item2 = reinterpret_cast<CLayoutEqualizerItem *>(item);
        item2->le = d;
        d->items.insert(item2);
    }
    QBoxLayout::addItem(item);
}

QLayoutItem *CEqualBoxLayout::takeAt(int index) {
    Q_D(CEqualBoxLayout);
    auto item = QBoxLayout::takeAt(index);
    if (typeid(*item) == typeid(CLayoutEqualizerItem)) {
        auto item2 = reinterpret_cast<CLayoutEqualizerItem *>(item);
        item2->le = nullptr;
        d->items.remove(item2);
    }
    return item;
}
