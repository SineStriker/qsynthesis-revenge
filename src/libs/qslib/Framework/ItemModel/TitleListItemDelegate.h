#ifndef TITLELISTITEMDELEGATE_H
#define TITLELISTITEMDELEGATE_H

#include <QStyledItemDelegate>

#include <QTypeFace.h>

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
        QTypeFace idleShape() const;
        QTypeFace selectShape() const;
        QTypeFace underlineShape() const;

        QTypeFace titleShape() const;
        QTypeFace subtitleShape() const;
        QTypeFace descriptionShape() const;
        QTypeFace descriptionBackShape() const;

        QMargins titleMargins() const;
        QMargins subtitleMargins() const;
        QMargins descriptionMargins() const;
        QMargins iconMargins() const;
        QMargins margins() const;

        void setIdleShape(const QTypeFace &shape);
        void setSelectShape(const QTypeFace &shape);
        void setUnderlineShape(const QTypeFace &shape);
        void setTitleShape(const QTypeFace &shape);
        void setSubtitleShape(const QTypeFace &shape);
        void setDescriptionShape(const QTypeFace &shape);
        void setDescriptionBackShape(const QTypeFace &shape);

        void setTitleMargins(const QMargins &margins);
        void setSubtitleMargins(const QMargins &margins);
        void setDescriptionMargins(const QMargins &margins);
        void setIconMargins(const QMargins &margins);
        void setMargins(const QMargins &margins);

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