#ifndef ADDITIVETREEWIDGET_H
#define ADDITIVETREEWIDGET_H

#include <CTreeWidget.h>

#include "ItemModel/AceTreeModel.h"

class AceTreeWidgetPrivate;

class QMEDITING_EXPORT AceTreeWidget : public CTreeWidget {
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

#endif // ADDITIVETREEWIDGET_H