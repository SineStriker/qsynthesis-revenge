#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QListWidget>
#include <QMenu>

#include "FileListItemDelegate.h"
#include "QColorList.h"
#include "QTypeList.h"

class FileListWidget : public QListWidget {
    Q_OBJECT
    Q_PROPERTY(QTypeList styleData READ styleData WRITE setStyleData NOTIFY styleDataChanged)

public:
    explicit FileListWidget(QWidget *parent = nullptr);
    ~FileListWidget();

private:
    void init();

public:
    void insertFileItem(int row, const QIcon &icon, const QSize &size, int type,
                        const QString &filename, const QString &location, const QString &date);
    void addFileItem(const QIcon &icon, const QSize &size, int type, const QString &filename,
                     const QString &location, const QString &date);

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

signals:
    void contentsSizeUpdated(const QSize &size);
    void styleDataChanged();
};

#endif // FILELISTWIDGET_H
