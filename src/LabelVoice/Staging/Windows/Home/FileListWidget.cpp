#include "FileListWidget.h"
#include "QMarginsImpl.h"

#include <private/qlistwidget_p.h>

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
    QVariant var0;
    var0.setValue(m_delegate->m_selectType);
    QVariant var1;
    var1.setValue(m_delegate->m_underline);
    QVariant var2;
    var2.setValue(m_delegate->m_fileType);
    QVariant var3;
    var3.setValue(m_delegate->m_locType);
    QVariant var4;
    var4.setValue(m_delegate->m_dateType);
    QVariant var5;
    var5.setValue(m_delegate->m_fileMargins);
    QVariant var6;
    var6.setValue(m_delegate->m_locMargins);
    QVariant var7;
    var7.setValue(m_delegate->m_dateMargins);
    QVariant var8;
    var8.setValue(m_delegate->m_iconMargins);
    QVariant var9;
    var9.setValue(m_delegate->m_margins);
    QVariant var10;
    var10.setValue(m_delegate->m_idleType);
    return {var0, var1, var2, var3, var4, var5, var6, var7, var8, var9, var10};
}

void FileListWidget::setStyleData(const QTypeList &list) {
    if (list.size() >= 9) {
        QVariant var0 = list.at(0);
        QVariant var1 = list.at(1);
        QVariant var2 = list.at(2);
        QVariant var3 = list.at(3);
        QVariant var4 = list.at(4);
        QVariant var5 = list.at(5);
        QVariant var6 = list.at(6);
        QVariant var7 = list.at(7);
        QVariant var8 = list.at(8);
        QVariant var9 = list.at(9);
        QVariant var10 = list.at(10);
        if (var0.convert(qMetaTypeId<QTypeFace>())) {
            m_delegate->m_idleType = var0.value<QTypeFace>();
        }
        if (var0.convert(qMetaTypeId<QTypeFace>())) {
            m_delegate->m_selectType = var0.value<QTypeFace>();
        }
        if (var1.convert(qMetaTypeId<QTypeFace>())) {
            m_delegate->m_underline = var1.value<QTypeFace>();
        }
        if (var2.convert(qMetaTypeId<QTypeFace>())) {
            m_delegate->m_fileType = var2.value<QTypeFace>();
        }
        if (var3.convert(qMetaTypeId<QTypeFace>())) {
            m_delegate->m_locType = var3.value<QTypeFace>();
        }
        if (var4.convert(qMetaTypeId<QTypeFace>())) {
            m_delegate->m_dateType = var4.value<QTypeFace>();
        }
        if (var5.convert(qMetaTypeId<QMargins>())) {
            m_delegate->m_fileMargins = var5.value<QMargins>();
        }
        if (var6.convert(qMetaTypeId<QMargins>())) {
            m_delegate->m_locMargins = var6.value<QMargins>();
        }
        if (var7.convert(qMetaTypeId<QMargins>())) {
            m_delegate->m_dateMargins = var7.value<QMargins>();
        }
        if (var8.convert(qMetaTypeId<QMargins>())) {
            m_delegate->m_iconMargins = var8.value<QMargins>();
        }
        if (var9.convert(qMetaTypeId<QMargins>())) {
            m_delegate->m_margins = var9.value<QMargins>();
        }
        if (var10.convert(qMetaTypeId<QTypeFace>())) {
            m_delegate->m_idleType = var10.value<QTypeFace>();
        }
    }
    update();
    emit styleDataChanged();
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
