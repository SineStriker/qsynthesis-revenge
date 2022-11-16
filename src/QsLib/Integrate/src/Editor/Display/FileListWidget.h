#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QListWidget>
#include <QMenu>

#include "QColorList.h"
#include "QTypeList.h"

#include "qsintegrate_global.h"

class FileListItemDelegate;

class QSINTEGRATE_API FileListWidget : public QListWidget {
    Q_OBJECT
    Q_PROPERTY(QTypeList styleData READ styleData WRITE setStyleData NOTIFY styleDataChanged)

public:
    explicit FileListWidget(QWidget *parent = nullptr);
    ~FileListWidget();

    enum ItemData {
        Filename = Qt::UserRole + 1,
        Location,
        Date,
        Icon,
        IconSize,
        Type,
    };

private:
    void init();

public:
    void insertItem(int row, const QIcon &icon, const QSize &size, int type,
                    const QString &filename, const QString &location, const QString &date);
    void addItem(const QIcon &icon, const QSize &size, int type, const QString &filename,
                 const QString &location, const QString &date);

    // Remove base methods
    void insertItem(int row, QListWidgetItem *item) = delete;
    void insertItem(int row, const QString &label) = delete;
    void insertItems(int row, const QStringList &labels) = delete;
    void addItem(const QString &label) = delete;
    void addItem(QListWidgetItem *item) = delete;
    void addItems(const QStringList &labels) = delete;

public:
    QTypeList styleData() const;
    void setStyleData(const QTypeList &list);

    FileListItemDelegate *delegate() const;

    QSize contentsSize() const;

protected:
    FileListItemDelegate *m_delegate;
    QSize oldContentsSize;

    void updateGeometries() override;

    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    void _q_delegateClicked(const QModelIndex &index, int button);

signals:
    void itemClickedEx(const QModelIndex &index, int button);
    void contentsSizeUpdated(const QSize &size);
    void styleDataChanged();
};

#endif // FILELISTWIDGET_H
