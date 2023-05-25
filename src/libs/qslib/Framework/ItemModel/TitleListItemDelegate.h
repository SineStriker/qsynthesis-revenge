#ifndef TITLELISTITEMDELEGATE_H
#define TITLELISTITEMDELEGATE_H

#include <QStyledItemDelegate>

#include <QLineStyle.h>
#include <QRectStyle.h>
#include <QTypeFace.h>
#include <QTypeList.h>

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class TitleListItemDelegatePrivate;

    class QSFRAMEWORK_API TitleListItemDelegate : public QStyledItemDelegate {
        Q_OBJECT
        Q_DECLARE_PRIVATE(TitleListItemDelegate)
    public:
        explicit TitleListItemDelegate(QObject *parent = nullptr);
        ~TitleListItemDelegate();

    public:
        QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;
        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    public:
        QTypeList styleData() const;
        void setStyleData(const QTypeList &list);

    signals:
        void clicked(const QModelIndex &index, int button);

    protected:
        bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                         const QModelIndex &index) override;

    protected:
        TitleListItemDelegate(TitleListItemDelegatePrivate &d, QObject *parent = nullptr);

        QScopedPointer<TitleListItemDelegatePrivate> d_ptr;
    };

}

#endif // TITLELISTITEMDELEGATE_H