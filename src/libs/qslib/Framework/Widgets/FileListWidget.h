#ifndef FILELISTWIDGET_H
#define FILELISTWIDGET_H

#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QListWidget>
#include <QMenu>

#include "QColorList.h"
#include "QTypeList.h"

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class FileListWidgetPrivate;

    class QSFRAMEWORK_API FileListWidget : public QListWidget {
        Q_OBJECT
        Q_DECLARE_PRIVATE(FileListWidget)
        Q_PROPERTY(QTypeList styleData READ styleData WRITE setStyleData NOTIFY styleDataChanged)

    public:
        explicit FileListWidget(QWidget *parent = nullptr);
        ~FileListWidget();

    public:
        QTypeList styleData() const;
        void setStyleData(const QTypeList &list);

        QSize contentsSize() const;

    protected:
        QScopedPointer<FileListWidgetPrivate> d_ptr;

        void updateGeometries() override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    signals:
        void itemClickedEx(const QModelIndex &index, int button);
        void contentsSizeUpdated(const QSize &size);
        void styleDataChanged();
    };

}

#endif // FILELISTWIDGET_H
