#ifndef FILELISTITEMDELEGATE_H
#define FILELISTITEMDELEGATE_H

#include <QDebug>
#include <QEvent>
#include <QListWidget>
#include <QMenu>
#include <QMouseEvent>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QWidget>

#include "QTypeFace.h"

// Recent list items
class FileListItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit FileListItemDelegate(QObject *parent = nullptr);
    ~FileListItemDelegate();

    friend class FileListWidget;

    enum ItemDataUserRole {
        Filename = Qt::UserRole + 1,
        Location,
        Date,
        Icon,
        IconSize,
        Type,
    };

public:
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;

protected:
    QTypeFace m_idleType;
    QTypeFace m_selectType;
    QTypeFace m_underline;

    QTypeFace m_fileType;
    QTypeFace m_locType;
    QTypeFace m_dateType;

    QMargins m_fileMargins;
    QMargins m_locMargins;
    QMargins m_dateMargins;
    QMargins m_margins;

    QMargins m_iconMargins;

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

signals:
    void clicked(const QModelIndex &index, int button);
};

#endif // FILELISTITEMDELEGATE_H
