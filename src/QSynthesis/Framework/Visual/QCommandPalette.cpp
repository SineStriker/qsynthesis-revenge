#include "QCommandPalette.h"
#include "QCommandPaletteImpl/QCommandPaletteItem.h"
#include "QCommandPaletteImpl/QCommandPalette_p.h"

#include <QApplication>
#include <QEvent>
#include <QKeyEvent>

QCommandPalette::QCommandPalette(QWidget *parent)
    : QCommandPalette(*new QCommandPalettePrivate(), parent) {
}

QCommandPalette::~QCommandPalette() {
}

int QCommandPalette::addWidget(QCommandPaletteItemWidget *w) {
    return insertWidget(count(), w);
}

int QCommandPalette::insertWidget(int index, QCommandPaletteItemWidget *w) {
    Q_D(QCommandPalette);
    auto item = new QListWidgetItem(d->listWidget);

    index = qMax(0, index);
    index = qMin(d->listWidget->count(), index);

    w->item = item;
    d->listWidget->insertItem(index, item);
    d->listWidget->setItemWidget(item, w);

    return index;
}

QCommandPaletteItemWidget *QCommandPalette::takeWidget(int index) {
    Q_D(QCommandPalette);
    auto item = d->listWidget->takeItem(index);
    if (!item) {
        return nullptr;
    }
    return qobject_cast<QCommandPaletteItemWidget *>(d->listWidget->itemWidget(item));
}

int QCommandPalette::count() const {
    Q_D(const QCommandPalette);
    return d->listWidget->count();
}

void QCommandPalette::showEvent(QShowEvent *event) {
    Q_UNUSED(event);
    setFocus();
}

bool QCommandPalette::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress || event->type() == QEvent::ShortcutOverride) {
        Q_D(QCommandPalette);
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        int key = keyEvent->key();
        if (key == Qt::Key_Up || key == Qt::Key_Down) {
            if (obj != d->listWidget) {
                d->listWidget->setFocus();
                QApplication::sendEvent(d->listWidget, keyEvent);
                return true;
            }
        } else if (key == Qt::Key_Return || key == Qt::Key_Enter) {
            int index = d->listWidget->currentRow();
            if (index >= 0 && index < d->listWidget->count()) {
                emit activated(index);
            }
            return true;
        } else if (key == Qt::Key_Tab) {
            return true;
        } else if (key == Qt::Key_Escape) {
            emit abandoned();
            return true;
        } else {
            if (obj != d->lineEdit) {
                d->lineEdit->setFocus();
                QApplication::sendEvent(d->lineEdit, keyEvent);
                return true;
            }
        }
    }
    return QWidget::eventFilter(obj, event);
}

QCommandPalette::QCommandPalette(QCommandPalettePrivate &d, QWidget *parent)
    : QWidget(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}

void QCommandPalette::_q_textChanged(const QString &text) {
}

void QCommandPalette::_q_currentRowChanged(int row) {
}

void QCommandPalette::_q_itemClicked(QListWidgetItem *item) {
}
