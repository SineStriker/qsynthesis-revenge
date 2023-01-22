#include "CNavFrame.h"

#include <QButtonGroup>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedLayout>

class CNavFramePrivate {
public:
    CNavFrame *q_ptr;

    QHBoxLayout *layout;
    QVBoxLayout *leftLayout;
    QVBoxLayout *buttonLayout;

    QWidget *topWidget;
    QWidget *bottomWidget;

    QStackedLayout *stack;
    QButtonGroup *btnGroup;
};

CNavFrame::CNavFrame(QWidget *parent) : QFrame(parent), d_ptr(new CNavFramePrivate()) {
    auto &d = *d_ptr.data();
    d.q_ptr = this;

    // Sub layouts
    d.buttonLayout = new QVBoxLayout();
    d.buttonLayout->setMargin(0);
    d.buttonLayout->setSpacing(0);

    d.stack = new QStackedLayout();

    d.leftLayout = new QVBoxLayout();
    d.leftLayout->setMargin(0);
    d.leftLayout->setSpacing(0);

    d.leftLayout->addLayout(d.buttonLayout);
    d.leftLayout->addStretch();

    // Main layout
    d.layout = new QHBoxLayout();
    d.layout->setMargin(0);
    d.layout->setSpacing(0);

    d.layout->addLayout(d.leftLayout);
    d.layout->addLayout(d.stack);

    setLayout(d.layout);

    d.btnGroup = new QButtonGroup(this);
    d.btnGroup->setExclusive(true);

    d.topWidget = nullptr;
    d.bottomWidget = nullptr;

    connect(d.stack, &QStackedLayout::currentChanged, this, &CNavFrame::_q_currentChanged);
    connect(d.btnGroup, &QButtonGroup::idClicked, this, &CNavFrame::_q_buttonClicked);
}

CNavFrame::~CNavFrame() {
}

QWidget *CNavFrame::topWidget() const {
    return d_ptr->topWidget;
}

void CNavFrame::setTopWidget(QWidget *w) {
    auto &d = *d_ptr.data();

    takeTopWidget();
    d.leftLayout->insertWidget(0, w);
    d.topWidget = w;
}

QWidget *CNavFrame::takeTopWidget() {
    auto &d = *d_ptr.data();
    QWidget *w = nullptr;
    if (d.topWidget) {
        w = d.topWidget;
        d_ptr->leftLayout->removeWidget(w);
        d.topWidget = nullptr;
    }
    return w;
}

QWidget *CNavFrame::bottomWidget() const {
    return d_ptr->bottomWidget;
}

void CNavFrame::setBottomWidget(QWidget *w) {
    auto &d = *d_ptr.data();

    takeBottomWidget();
    d.leftLayout->addWidget(w);
    d.bottomWidget = w;
}

QWidget *CNavFrame::takeBottomWidget() {
    auto &d = *d_ptr.data();
    QWidget *w = nullptr;
    if (d.bottomWidget) {
        w = d.bottomWidget;
        d_ptr->leftLayout->removeWidget(w);
        d.bottomWidget = nullptr;
    }
    return w;
}

QAbstractButton *CNavFrame::addWidget(QWidget *w) {
    return insertWidget(count(), w);
}

QAbstractButton *CNavFrame::insertWidget(int index, QWidget *w) {
    index = qMin(index, count());

    auto &d = *d_ptr.data();

    // New button
    auto btn = new QPushButton();
    btn->setCheckable(true);
    d.btnGroup->addButton(btn);

    // Insert to layouts
    d.buttonLayout->insertWidget(index, btn);
    d.stack->insertWidget(index, w);

    return btn;
}

void CNavFrame::removeWidget(QWidget *w) {
    removeAt(d_ptr->stack->indexOf(w));
}

void CNavFrame::removeAt(int index) {
    if (index >= count() || index < 0) {
        return;
    }
    auto &d = *d_ptr.data();

    // Find button
    auto btn = qobject_cast<QAbstractButton *>(d.buttonLayout->itemAt(index)->widget());
    d.btnGroup->removeButton(btn);

    // Remove from layouts
    d.buttonLayout->removeWidget(btn);
    d.stack->removeWidget(d.stack->widget(index));

    // Delete button
    btn->deleteLater();
}

QWidget *CNavFrame::widget(int index) {
    return d_ptr->stack->widget(index);
}

QWidget *CNavFrame::currentWidget() const {
    return d_ptr->stack->currentWidget();
}

int CNavFrame::currentIndex() const {
    return d_ptr->stack->currentIndex();
}

int CNavFrame::count() const {
    return d_ptr->stack->count();
}

int CNavFrame::indexOf(QWidget *w) const {
    return d_ptr->stack->indexOf(w);
}

void CNavFrame::setCurrentIndex(int index) {
    d_ptr->stack->setCurrentIndex(index);
}

void CNavFrame::setCurrentWidget(QWidget *w) {
    d_ptr->stack->setCurrentWidget(w);
}

void CNavFrame::_q_currentChanged(int index) {
    qobject_cast<QAbstractButton *>(d_ptr->buttonLayout->itemAt(index)->widget())->setChecked(true);
}

void CNavFrame::_q_buttonClicked(int id) {
    Q_UNUSED(id);

    auto &d = *d_ptr.data();
    setCurrentIndex(d.buttonLayout->indexOf(d.btnGroup->checkedButton()));
}
