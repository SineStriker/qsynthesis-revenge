#ifndef TITLELISTWIDGET_H
#define TITLELISTWIDGET_H

#include <QAction>
#include <QContextMenuEvent>
#include <QDebug>
#include <QListWidget>
#include <QMenu>

#include "QCssValueMap.h"

#include "QsFrameworkGlobal.h"

namespace QsApi {

    class TitleListWidgetPrivate;

    class QSFRAMEWORK_API TitleListWidget : public QListWidget {
        Q_OBJECT
        Q_DECLARE_PRIVATE(TitleListWidget)
        Q_PROPERTY(QCssValueMap styleData READ styleData WRITE setStyleData)
    public:
        explicit TitleListWidget(QWidget *parent = nullptr);
        ~TitleListWidget();

    public:
        QCssValueMap styleData() const;
        void setStyleData(const QCssValueMap &map);

        QSize contentsSize() const;

    protected:
        QScopedPointer<TitleListWidgetPrivate> d_ptr;

        void updateGeometries() override;
        void mouseReleaseEvent(QMouseEvent *event) override;

    signals:
        void itemClickedEx(const QModelIndex &index, int button);
        void contentsSizeUpdated(const QSize &size);
    };

}

#endif // TITLELISTWIDGET_H
