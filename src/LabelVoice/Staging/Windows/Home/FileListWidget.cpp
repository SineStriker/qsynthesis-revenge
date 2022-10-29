#include "FileListWidget.h"
#include "QMarginsImpl.h"

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
    m_delegate = new FileListItemDelegate(this);

    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);

    setItemDelegate(m_delegate);
    setSelectionMode(QAbstractItemView::SingleSelection);
}

void FileListWidget::addFileItem(const QIcon &icon, const QSize &size, int type,
                                 const QString &filename, const QString &location,
                                 const QString &date) {
    insertFileItem(count(), icon, size, type, filename, location, date);
}

void FileListWidget::insertFileItem(int row, const QIcon &icon, const QSize &size, int type,
                                    const QString &filename, const QString &location,
                                    const QString &date) {
    QListWidgetItem *item = new QListWidgetItem();
    item->setData(FileListItemDelegate::Filename, filename);
    item->setData(FileListItemDelegate::Location, location);
    item->setData(FileListItemDelegate::Date, date);
    item->setData(FileListItemDelegate::Icon, icon);
    item->setData(FileListItemDelegate::IconSize, size);
    item->setData(FileListItemDelegate::Type, type);
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
