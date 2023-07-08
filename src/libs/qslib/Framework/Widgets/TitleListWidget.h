#ifndef TITLELISTWIDGET_H
#define TITLELISTWIDGET_H

#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QListWidget>
#include <QMenu>

#include "QTypeMap.h"

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class TitleListWidgetPrivate;

    class QSFRAMEWORK_API TitleListWidget : public QListWidget {
        Q_OBJECT
        Q_DECLARE_PRIVATE(TitleListWidget)
        Q_PROPERTY(QTypeMap styleData READ styleData WRITE setStyleData NOTIFY styleDataChanged)
    public:
        explicit TitleListWidget(QWidget *parent = nullptr);
        ~TitleListWidget();

    public:
        QTypeMap styleData() const;
        void setStyleData(const QTypeMap &map);

        QSize contentsSize() const;

    protected:
        QScopedPointer<TitleListWidgetPrivate> d_ptr;

        void updateGeometries() override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    signals:
        void itemClickedEx(const QModelIndex &index, int button);
        void contentsSizeUpdated(const QSize &size);
        void styleDataChanged();
    };

}

#endif // TITLELISTWIDGET_H
