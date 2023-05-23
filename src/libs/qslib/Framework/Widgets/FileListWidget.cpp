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

    QTypeList FileListWidgetPrivate::styleData_helper() const {
        return {
            QVariant::fromValue(m_delegate->idleShape()),
            QVariant::fromValue(m_delegate->selectShape()),
            QVariant::fromValue(m_delegate->underlineShape()),
            QVariant::fromValue(m_delegate->titleShape()),
            QVariant::fromValue(m_delegate->subtitleShape()),
            QVariant::fromValue(m_delegate->descriptionShape()),

            QVariant::fromValue(m_delegate->titleMargins()),
            QVariant::fromValue(m_delegate->subtitleMargins()),
            QVariant::fromValue(m_delegate->descriptionMargins()),
            QVariant::fromValue(m_delegate->iconMargins()),
            QVariant::fromValue(m_delegate->margins()),
        };
    }

    void FileListWidgetPrivate::setStyleData_helper(const QTypeList &list) {
        Q_Q(FileListWidget);
        if (list.size() >= 11) {
            int i = 0;

            decodeStyle<QTypeFace>(list.at(i++), &TitleListItemDelegate::setIdleShape);
            decodeStyle<QTypeFace>(list.at(i++), &TitleListItemDelegate::setSelectShape);
            decodeStyle<QTypeFace>(list.at(i++), &TitleListItemDelegate::setUnderlineShape);
            decodeStyle<QTypeFace>(list.at(i++), &TitleListItemDelegate::setTitleShape);
            decodeStyle<QTypeFace>(list.at(i++), &TitleListItemDelegate::setSubtitleShape);
            decodeStyle<QTypeFace>(list.at(i++), &TitleListItemDelegate::setDescriptionShape);

            decodeStyle<QMargins>(list.at(i++), &TitleListItemDelegate::setTitleMargins);
            decodeStyle<QMargins>(list.at(i++), &TitleListItemDelegate::setSubtitleMargins);
            decodeStyle<QMargins>(list.at(i++), &TitleListItemDelegate::setDescriptionMargins);
            decodeStyle<QMargins>(list.at(i++), &TitleListItemDelegate::setIconMargins);
            decodeStyle<QMargins>(list.at(i++), &TitleListItemDelegate::setMargins);

            auto shape = m_delegate->titleShape();
            if (shape.isDefaultFont()) {
                shape.setFont(q->font());
                m_delegate->setTitleShape(shape);
            }

            shape = m_delegate->subtitleShape();
            if (shape.isDefaultFont()) {
                shape.setFont(q->font());
                m_delegate->setSubtitleShape(shape);
            }

            shape = m_delegate->descriptionShape();
            if (shape.isDefaultFont()) {
                shape.setFont(q->font());
                m_delegate->setDescriptionShape(shape);
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
