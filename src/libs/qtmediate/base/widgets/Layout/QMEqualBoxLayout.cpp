#include "QMEqualBoxLayout.h"

#include <QDebug>
#include <QSet>
#include <QWidget>

#include <private/qlayout_p.h>

class CLayoutEqualizerItem;

// EqualizerPrivate
class QMEqualBoxLayoutPrivate {
public:
    QMEqualBoxLayoutPrivate(QMEqualBoxLayout *q) : q_ptr(q){};

    QSet<CLayoutEqualizerItem *> items;

    QMEqualBoxLayout *q_ptr;
};

// Item
class CLayoutEqualizerItem : public QWidgetItemV2 {
public:
    explicit CLayoutEqualizerItem(QWidget *widget);
    ~CLayoutEqualizerItem();

protected:
    QSize sizeHint() const override;

    QSize realSizeHint() const;

    QMEqualBoxLayoutPrivate *le;

    friend class QMEqualBoxLayout;
};

CLayoutEqualizerItem::CLayoutEqualizerItem(QWidget *widget) : QWidgetItemV2(widget) {
}

CLayoutEqualizerItem::~CLayoutEqualizerItem() {
}

QSize CLayoutEqualizerItem::sizeHint() const {
    auto rs = realSizeHint();
    if (!le || rs.width() == 0 || rs.width() == 0) {
        return rs;
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
QMEqualBoxLayout::QMEqualBoxLayout(Direction direction, QWidget *parent)
    : QBoxLayout(direction, parent), d_ptr(new QMEqualBoxLayoutPrivate(this)) {
}

QMEqualBoxLayout::~QMEqualBoxLayout() {
}

void QMEqualBoxLayout::addWidgetE(QWidget *widget, int stretch, Qt::Alignment alignment) {
    insertWidgetE(-1, widget, stretch, alignment);
}

void QMEqualBoxLayout::insertWidgetE(int index, QWidget *widget, int stretch, Qt::Alignment alignment) {
    auto org = QLayoutPrivate::widgetItemFactoryMethod;

    // Exchange method temporarily
    QLayoutPrivate::widgetItemFactoryMethod = [](const QLayout *layout, QWidget *widget) -> QWidgetItem * {
        auto item = new CLayoutEqualizerItem(widget);
        item->le = reinterpret_cast<const QMEqualBoxLayout *>(layout)->d_ptr.data();
        item->le->items.insert(item);
        return item;
    };

    QBoxLayout::insertWidget(index, widget, stretch, alignment);

    QLayoutPrivate::widgetItemFactoryMethod = org;
}

void QMEqualBoxLayout::addItem(QLayoutItem *item) {
    Q_D(QMEqualBoxLayout);
    if (typeid(*item) == typeid(CLayoutEqualizerItem)) {
        auto item2 = reinterpret_cast<CLayoutEqualizerItem *>(item);
        item2->le = d;
        d->items.insert(item2);
    }
    QBoxLayout::addItem(item);
}

QLayoutItem *QMEqualBoxLayout::takeAt(int index) {
    Q_D(QMEqualBoxLayout);
    auto item = QBoxLayout::takeAt(index);
    if (typeid(*item) == typeid(CLayoutEqualizerItem)) {
        auto item2 = reinterpret_cast<CLayoutEqualizerItem *>(item);
        item2->le = nullptr;
        d->items.remove(item2);
    }
    return item;
}
