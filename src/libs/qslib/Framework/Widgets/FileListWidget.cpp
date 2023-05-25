#include "FileListWidget.h"
#include "FileListWidget_p.h"

#include "QMarginsImpl.h"

#include <private/qlistwidget_p.h>

namespace QsApi {

    FileListWidgetPrivate::FileListWidgetPrivate() {
    }

    FileListWidgetPrivate::~FileListWidgetPrivate() {
    }

    void FileListWidgetPrivate::init() {
        Q_Q(FileListWidget);

        q->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        q->setSelectionMode(QAbstractItemView::SingleSelection);

        m_delegate = new TitleListItemDelegate(this);
        q->setItemDelegate(m_delegate);

        connect(m_delegate, &TitleListItemDelegate::clicked, this, &FileListWidgetPrivate::_q_delegateClicked);
    }

    void FileListWidgetPrivate::_q_delegateClicked(const QModelIndex &index, int button) {
        Q_Q(FileListWidget);
        emit q->itemClickedEx(index, button);
    }

    FileListWidget::FileListWidget(QWidget *parent) : QListWidget(parent), d_ptr(new FileListWidgetPrivate()) {
        Q_D(FileListWidget);
        d->q_ptr = this;
        d->init();
    }

    FileListWidget::~FileListWidget() {
    }

    QTypeList FileListWidget::styleData() const {
        Q_D(const FileListWidget);
        return d->m_delegate->styleData();
    }

    void FileListWidget::setStyleData(const QTypeList &list) {
        Q_D(FileListWidget);
        d->m_delegate->setStyleData(list);
        update();

        emit styleDataChanged();
    }

    QSize FileListWidget::contentsSize() const {
        return static_cast<QListWidgetPrivate *>(QListWidget::d_ptr.data())->contentsSize();
    }

    void FileListWidget::updateGeometries() {
        Q_D(FileListWidget);

        QListWidget::updateGeometries();

        QSize size = contentsSize();
        if (d->oldContentsSize != size) {
            d->oldContentsSize = size;
            emit contentsSizeUpdated(size);
        }
    }

    void FileListWidget::mouseReleaseEvent(QMouseEvent *event) {
        QListWidget::mouseReleaseEvent(event);
        clearSelection();
    }

}
