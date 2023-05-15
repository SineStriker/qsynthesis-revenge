#include "QScrollableTabWidget.h"
#include "private/QScrollableTabWidget_p.h"

#include "private/QScrollableTabBar_p.h"

#include <QApplication>
#include <QDebug>
#include <QDragEnterEvent>
#include <QDropEvent>

static inline QString id_format() {
    return QString("application/%1.pid%2.data")
        .arg(qApp->applicationName(), QString::number(qApp->applicationPid()));
}

QScrollableTabWidget::QScrollableTabWidget(QWidget *parent)
    : QScrollableTabWidget(*new QScrollableTabWidgetPrivate(), parent) {
}

QScrollableTabWidget::~QScrollableTabWidget() {
}

QScrollableTabBar *QScrollableTabWidget::tabBar() const {
    Q_D(const QScrollableTabWidget);
    return d->tabBar;
}

int QScrollableTabWidget::addTab(QWidget *widget, const QString &label) {
    return insertTab(-1, widget, label);
}

int QScrollableTabWidget::addTab(QWidget *widget, const QIcon &icon, const QString &label) {
    return insertTab(-1, widget, icon, label);
}

int QScrollableTabWidget::insertTab(int index, QWidget *widget, const QString &label) {
    return insertTab(index, widget, QIcon(), label);
}

int QScrollableTabWidget::insertTab(int index, QWidget *widget, const QIcon &icon,
                                    const QString &label) {
    Q_D(QScrollableTabWidget);
    if (!widget) {
        return -1;
    }

    index = d->stack->insertWidget(index, widget);
    d->tabBar->insertTab(index, icon, label);
    tabInserted(index);
    return index;
}

void QScrollableTabWidget::removeTab(int index) {
    Q_D(QScrollableTabWidget);
    if (QWidget *w = d->stack->widget(index)) {
        d->stack->removeWidget(w);
    }
}

QString QScrollableTabWidget::tabText(int index) const {
    Q_D(const QScrollableTabWidget);
    return d->tabBar->tabText(index);
}

void QScrollableTabWidget::setTabText(int index, const QString &label) {
    Q_D(QScrollableTabWidget);
    d->tabBar->setTabText(index, label);
}

QIcon QScrollableTabWidget::tabIcon(int index) const {
    Q_D(const QScrollableTabWidget);
    return d->tabBar->tabIcon(index);
}

void QScrollableTabWidget::setTabIcon(int index, const QIcon &icon) {
    Q_D(QScrollableTabWidget);
    d->tabBar->setTabIcon(index, icon);
}

QString QScrollableTabWidget::tabToolTip(int index) const {
    Q_D(const QScrollableTabWidget);
    return d->tabBar->tabToolTip(index);
}

void QScrollableTabWidget::setTabToolTip(int index, const QString &tip) {
    Q_D(QScrollableTabWidget);
    d->tabBar->setTabToolTip(index, tip);
}

QVariant QScrollableTabWidget::tabData(int index) const {
    Q_D(const QScrollableTabWidget);
    return d->tabBar->tabData(index);
}

void QScrollableTabWidget::setTabData(int index, const QVariant &data) {
    Q_D(QScrollableTabWidget);
    d->tabBar->setTabData(index, data);
}

int QScrollableTabWidget::currentIndex() const {
    Q_D(const QScrollableTabWidget);
    return d->tabBar->currentIndex();
}

void QScrollableTabWidget::setCurrentIndex(int index) {
    Q_D(QScrollableTabWidget);
    d->tabBar->setCurrentIndex(index);
}

QWidget *QScrollableTabWidget::currentWidget() const {
    Q_D(const QScrollableTabWidget);
    return d->stack->currentWidget();
}

void QScrollableTabWidget::setCurrentWidget(QWidget *widget) {
    Q_D(const QScrollableTabWidget);
    d->tabBar->setCurrentIndex(indexOf(widget));
}

QWidget *QScrollableTabWidget::widget(int index) const {
    Q_D(const QScrollableTabWidget);
    return d->stack->widget(index);
}

int QScrollableTabWidget::indexOf(QWidget *w) const {
    Q_D(const QScrollableTabWidget);
    return d->stack->indexOf(w);
}

int QScrollableTabWidget::count() const {
    Q_D(const QScrollableTabWidget);
    return d->tabBar->count();
}

QSize QScrollableTabWidget::iconSize(int index) const {
    return d_func()->tabBar->iconSize(index);
}

void QScrollableTabWidget::setIconSize(int index, const QSize &size) {
    d_func()->tabBar->setIconSize(index, size);
}

void QScrollableTabWidget::clear() {
    while (count() != 0) {
        removeTab(0);
    }
}

void QScrollableTabWidget::tabInserted(int index) {
    Q_UNUSED(index)
}

void QScrollableTabWidget::tabRemoved(int index) {
    Q_UNUSED(index)
}

void QScrollableTabWidget::tabSelected(int index, int orgIndex) {
    Q_UNUSED(index)
    Q_UNUSED(orgIndex)
}

bool QScrollableTabWidget::tabIncoming(const QMimeData *mime) const {
    Q_UNUSED(mime);
    return true;
}

void QScrollableTabWidget::setTabBar(QScrollableTabBar *tabBar) {
    Q_D(QScrollableTabWidget);

    if (d->tabBar) {
        d->barLayout->replaceWidget(d->tabBar, tabBar);
        d->tabBar->deleteLater();
    } else {
        d->barLayout->insertWidget(0, tabBar);
        tabBar->d_func()->tabs = this;
    }

    d->tabBar = tabBar;
    tabBar->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    tabBar->show();
    setFocusProxy(tabBar);

    connect(d->tabBar, &QScrollableTabBar::currentChanged, this, &QScrollableTabWidget::_q_showTab);
    connect(d->tabBar, &QScrollableTabBar::tabMoved, this, &QScrollableTabWidget::_q_tabMoved);

    // Proxy Signals
    connect(d->tabBar, &QScrollableTabBar::tabBarClicked, this,
            &QScrollableTabWidget::tabBarClicked);
    connect(d->tabBar, &QScrollableTabBar::tabCloseRequested, this,
            &QScrollableTabWidget::tabCloseRequested);
}

void QScrollableTabWidget::dragEnterEvent(QDragEnterEvent *event) {
    const QMimeData *mime = event->mimeData();
    QString idFormat = id_format();
    if (mime->hasFormat(idFormat) && !mime->data(idFormat).compare(metaObject()->className())) {
        event->acceptProposedAction();
        return;
    }
    return QWidget::dragEnterEvent(event);
}

void QScrollableTabWidget::dropEvent(QDropEvent *event) {
    const QMimeData *mime = event->mimeData();
    QString idFormat = id_format();
    if (mime->hasFormat(idFormat) && !mime->data(idFormat).compare(metaObject()->className())) {
        auto orgBar = QScrollableTabBar::currentDraggedTabBar();
        auto orgTabs = orgBar->d_func()->tabs;
        int orgIndex;
        if (orgBar && orgTabs && (orgIndex = orgBar->currentDraggedIndex()) >= 0 &&
            tabIncoming(mime)) {
            // Calculate New Index
            auto bar = tabBar();
            QPoint p = bar->mapFromGlobal(QCursor::pos());
            int index = bar->tabAt(p);
            if (index < 0 && (p.y() >= 0 && p.y() <= bar->height())) {
                if (p.x() < 0) {
                    index = 0;
                } else if (p.x() > bar->totalWidth()) {
                    index = bar->count();
                }
            }
            if (index < 0) {
                if (orgBar != bar) {
                    index = count();
                } else {
                    index = orgIndex;
                }
            }

            if (orgBar == bar) {
                if (index != orgIndex) {
                    if (index >= bar->count()) {
                        index = bar->count() - 1;
                    }
                    bar->moveTab(orgIndex, index);
                }
            } else {
                // Record And Remove
                auto tab = orgTabs->widget(orgIndex);
                auto icon = orgBar->tabIcon(orgIndex);
                auto label = orgBar->tabText(orgIndex);
                orgTabs->removeTab(orgIndex);

                // Insert And Switch
                insertTab(index, tab, icon, label);
            }

            setCurrentIndex(index);
            event->acceptProposedAction();
            return;
        }
    }
    return QWidget::dropEvent(event);
}

QScrollableTabWidget::QScrollableTabWidget(QScrollableTabWidgetPrivate &d, QWidget *parent)
    : QFrame(parent), d_ptr(&d) {
    d.q_ptr = this;
    d.init();
}

QAbstractButton *QScrollableTabWidget::closeButton(int index) const {
    Q_D(const QScrollableTabWidget);
    return d->tabBar->closeButton(index);
}

void QScrollableTabWidget::setCloseButton(int index, QAbstractButton *button) {
    Q_D(const QScrollableTabWidget);
    d->tabBar->setCloseButton(index, button);
}

QWidget *QScrollableTabWidget::tabBarWidget() {
    Q_D(const QScrollableTabWidget);
    return d->tabBarWidget;
}

QWidget *QScrollableTabWidget::moveOutTabBarWidget() {
    Q_D(QScrollableTabWidget);
    auto w = d->tabBarWidget;
    if (w->parentWidget() != this) {
        return nullptr;
    }
    d->mainLayout->removeWidget(w);
    return w;
}

void QScrollableTabWidget::moveInTabBarWidget() {
    Q_D(const QScrollableTabWidget);
    auto w = d->tabBarWidget;
    if (w->parentWidget() == this) {
        return;
    }
    d->mainLayout->insertWidget(0, d->tabBarWidget);
}

void QScrollableTabWidget::_q_showTab(int index, int orgIndex) {
    Q_D(QScrollableTabWidget);
    if (index >= 0 && index < d->stack->count()) {
        d->stack->setCurrentIndex(index);
        tabSelected(index, orgIndex);
    }
    emit currentChanged(index, orgIndex);
}

void QScrollableTabWidget::_q_removeTab(int index) {
    Q_D(QScrollableTabWidget);
    d->tabBar->removeTab(index);
    tabRemoved(index);
}

void QScrollableTabWidget::_q_tabMoved(int from, int to) {
    Q_D(QScrollableTabWidget);
    const QSignalBlocker blocker(d->stack);
    QWidget *w = d->stack->widget(from);
    d->stack->removeWidget(w);
    d->stack->insertWidget(to, w);
}
