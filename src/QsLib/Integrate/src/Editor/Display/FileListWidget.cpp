#include "FileListWidget.h"
#include "QMarginsImpl.h"

#include "private/FileListItemDelegate.h"

#include <private/qlistwidget_p.h>

#define DECODE_STYLE(VAR, VARIANT, TYPE)                                                           \
    {                                                                                              \
        QVariant var = VARIANT;                                                                    \
        if (var.convert(qMetaTypeId<TYPE>())) {                                                    \
            VAR = var.value<TYPE>();                                                               \
        }                                                                                          \
    }

FileListWidget::FileListWidget(QWidget *parent) : QListWidget(parent) {
    init();
}

FileListWidget::~FileListWidget() {
}

void FileListWidget::init() {
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    setSelectionMode(QAbstractItemView::SingleSelection);

    m_delegate = new FileListItemDelegate(this);
    setItemDelegate(m_delegate);

    connect(m_delegate, &FileListItemDelegate::clicked, this, &FileListWidget::itemClickedEx);
}

void FileListWidget::addItem(const QIcon &icon, const QSize &size, int type,
                                 const QString &filename, const QString &location,
                                 const QString &date) {
    insertItem(count(), icon, size, type, filename, location, date);
}

void FileListWidget::insertItem(int row, const QIcon &icon, const QSize &size, int type,
                                    const QString &filename, const QString &location,
                                    const QString &date) {
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(FileListWidget::Filename, filename);
    item->setData(FileListWidget::Location, location);
    item->setData(FileListWidget::Date, date);
    item->setData(FileListWidget::Icon, icon);
    item->setData(FileListWidget::IconSize, size);
    item->setData(FileListWidget::Type, type);
    QListWidget::insertItem(row, item);
}

QTypeList FileListWidget::styleData() const {
    return {
        QVariant::fromValue(m_delegate->m_idleType),
        QVariant::fromValue(m_delegate->m_selectType),
        QVariant::fromValue(m_delegate->m_underline),
        QVariant::fromValue(m_delegate->m_fileType),
        QVariant::fromValue(m_delegate->m_locType),
        QVariant::fromValue(m_delegate->m_dateType),
        QVariant::fromValue(m_delegate->m_fileMargins),
        QVariant::fromValue(m_delegate->m_locMargins),
        QVariant::fromValue(m_delegate->m_dateMargins),
        QVariant::fromValue(m_delegate->m_iconMargins),
        QVariant::fromValue(m_delegate->m_margins),
    };
}

void FileListWidget::setStyleData(const QTypeList &list) {
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

        update();
        emit styleDataChanged();
    }
}

FileListItemDelegate *FileListWidget::delegate() const {
    return m_delegate;
}

QSize FileListWidget::contentsSize() const {
    auto d = static_cast<QListWidgetPrivate *>(d_ptr.data());
    return d->contentsSize();
}

void FileListWidget::updateGeometries() {
    QListWidget::updateGeometries();

    QSize size = contentsSize();
    if (oldContentsSize != size) {
        oldContentsSize = size;
        emit contentsSizeUpdated(size);
    }
}

void FileListWidget::mouseReleaseEvent(QMouseEvent *event) {
    QListWidget::mouseReleaseEvent(event);
    clearSelection();
}

void FileListWidget::_q_delegateClicked(const QModelIndex &index, int button) {
    emit itemClickedEx(index, button);
}
