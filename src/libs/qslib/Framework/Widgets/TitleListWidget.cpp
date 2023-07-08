#include "TitleListWidget.h"
#include "TitleListWidget_p.h"

#include "QMarginsImpl.h"

#include <private/qlistwidget_p.h>

namespace QsApi {

    TitleListWidgetPrivate::TitleListWidgetPrivate() {
    }

    TitleListWidgetPrivate::~TitleListWidgetPrivate() {
    }

    void TitleListWidgetPrivate::init() {
        Q_Q(TitleListWidget);

        q->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        q->setSelectionMode(QAbstractItemView::SingleSelection);

        m_delegate = new TitleListItemDelegate(this);
        q->setItemDelegate(m_delegate);

#ifndef Q_OS_WINDOWS
        q->setMouseTracking(true);
#endif

        connect(m_delegate, &TitleListItemDelegate::clicked, this, &TitleListWidgetPrivate::_q_delegateClicked);
    }

    void TitleListWidgetPrivate::_q_delegateClicked(const QModelIndex &index, int button) {
        Q_Q(TitleListWidget);
        emit q->itemClickedEx(index, button);
    }

    TitleListWidget::TitleListWidget(QWidget *parent) : QListWidget(parent), d_ptr(new TitleListWidgetPrivate()) {
        Q_D(TitleListWidget);
        d->q_ptr = this;
        d->init();
    }

    TitleListWidget::~TitleListWidget() {
    }

    QTypeMap TitleListWidget::styleData() const {
        Q_D(const TitleListWidget);
        return d->m_delegate->styleData();
    }

    void TitleListWidget::setStyleData(const QTypeMap &map) {
        Q_D(TitleListWidget);
        d->m_delegate->setStyleData(map);
        update();

        emit styleDataChanged();
    }

    QSize TitleListWidget::contentsSize() const {
        return static_cast<QListWidgetPrivate *>(QListWidget::d_ptr.data())->contentsSize();
    }

    void TitleListWidget::updateGeometries() {
        Q_D(TitleListWidget);

        QListWidget::updateGeometries();

        QSize size = contentsSize();
        if (d->oldContentsSize != size) {
            d->oldContentsSize = size;
            emit contentsSizeUpdated(size);
        }
    }

    void TitleListWidget::mouseReleaseEvent(QMouseEvent *event) {
        QListWidget::mouseReleaseEvent(event);
        clearSelection();
    }

}
