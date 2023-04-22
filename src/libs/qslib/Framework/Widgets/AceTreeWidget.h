#ifndef ADDITIVETREEWIDGET_H
#define ADDITIVETREEWIDGET_H

#include <CTreeWidget.h>

#include "ItemModel/AceTreeModel.h"

namespace QsApi {

    class AceTreeWidgetPrivate;

    class QSFRAMEWORK_API AceTreeWidget : public CTreeWidget {
        Q_OBJECT
        Q_DECLARE_PRIVATE(AceTreeWidget)
    public:
        explicit AceTreeWidget(QWidget *parent = nullptr);
        ~AceTreeWidget();

    public:
        AceTreeModel *model() const;

    protected:
        AceTreeWidget(AceTreeWidgetPrivate &d, QWidget *parent = nullptr);

        QScopedPointer<AceTreeWidgetPrivate> d_ptr;
    };

}

#endif // ADDITIVETREEWIDGET_H