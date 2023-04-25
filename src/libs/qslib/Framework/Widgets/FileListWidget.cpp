#include "FileListWidget.h"
#include "QMarginsImpl.h"

#include "private/FileListItemDelegate.h"
#include "private/FileListWidget_p.h"

#include <private/qlistwidget_p.h>

#define DECODE_STYLE(VAR, VARIANT, TYPE)                                                                               \
    {                                                                                                                  \
        QVariant var = VARIANT;                                                                                        \
        if (var.convert(qMetaTypeId<TYPE>())) {                                                                        \
            VAR = var.value<TYPE>();                                                                                   \
        }                                                                                                              \
    }

namespace QsApi {

    FileListWidgetPrivate::FileListWidgetPrivate() {
    }

    FileListWidgetPrivate::~FileListWidgetPrivate() {
    }

    void FileListWidgetPrivate::init() {
        Q_Q(FileListWidget);

        q->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
        q->setSelectionMode(QAbstractItemView::SingleSelection);

        m_delegate = new FileListItemDelegate(this);
        q->setItemDelegate(m_delegate);

        connect(m_delegate, &FileListItemDelegate::clicked, this, &FileListWidgetPrivate::_q_delegateClicked);
    }

    QTypeList FileListWidgetPrivate::styleData_helper() const {
        return {
            QVariant::fromValue(m_delegate->m_idleType),    QVariant::fromValue(m_delegate->m_selectType),
            QVariant::fromValue(m_delegate->m_underline),   QVariant::fromValue(m_delegate->m_fileType),
            QVariant::fromValue(m_delegate->m_locType),     QVariant::fromValue(m_delegate->m_dateType),
            QVariant::fromValue(m_delegate->m_fileMargins), QVariant::fromValue(m_delegate->m_locMargins),
            QVariant::fromValue(m_delegate->m_dateMargins), QVariant::fromValue(m_delegate->m_iconMargins),
            QVariant::fromValue(m_delegate->m_margins),
        };
    }

    void FileListWidgetPrivate::setStyleData_helper(const QTypeList &list) {
        Q_Q(FileListWidget);
        if (list.size() >= 11) {
            int i = 0;

            DECODE_STYLE(m_delegate->m_idleType, list.at(i++), QTypeFace);
            DECODE_STYLE(m_delegate->m_selectType, list.at(i++), QTypeFace);
            DECODE_STYLE(m_delegate->m_underline, list.at(i++), QTypeFace);
            DECODE_STYLE(m_delegate->m_fileType, list.at(i++), QTypeFace);
            DECODE_STYLE(m_delegate->m_locType, list.at(i++), QTypeFace);
            DECODE_STYLE(m_delegate->m_dateType, list.at(i++), QTypeFace);
            DECODE_STYLE(m_delegate->m_fileMargins, list.at(i++), QMargins);
            DECODE_STYLE(m_delegate->m_locMargins, list.at(i++), QMargins);
            DECODE_STYLE(m_delegate->m_dateMargins, list.at(i++), QMargins);
            DECODE_STYLE(m_delegate->m_iconMargins, list.at(i++), QMargins);
            DECODE_STYLE(m_delegate->m_margins, list.at(i++), QMargins);

            if (m_delegate->m_fileType.isDefaultFont()) {
                m_delegate->m_fileType.setFont(q->font());
            }
            if (m_delegate->m_locType.isDefaultFont()) {
                m_delegate->m_locType.setFont(q->font());
            }
            if (m_delegate->m_dateType.isDefaultFont()) {
                m_delegate->m_dateType.setFont(q->font());
            }

            q->update();

            emit q->styleDataChanged();
        }
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

    void FileListWidget::addItem(const QIcon &icon, const QSize &size, int type, const QString &filename,
                                 const QString &date) {
        insertItem(count(), icon, size, type, filename, date);
    }

    void FileListWidget::insertItem(int row, const QIcon &icon, const QSize &size, int type, const QString &filename,
                                    const QString &date) {
        auto item = new QListWidgetItem();
        item->setData(FileListWidget::Filename, filename);
        item->setData(FileListWidget::Date, date);
        item->setData(FileListWidget::Icon, icon);
        item->setData(FileListWidget::IconSize, size);
        item->setData(FileListWidget::Type, type);
        QListWidget::insertItem(row, item);
    }

    QTypeList FileListWidget::styleData() const {
        Q_D(const FileListWidget);
        return d->styleData_helper();
    }

    void FileListWidget::setStyleData(const QTypeList &list) {
        Q_D(FileListWidget);
        d->setStyleData_helper(list);
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
