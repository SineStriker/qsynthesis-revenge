#ifndef CTREEVIEW_H
#define CTREEVIEW_H

#include <QTreeView>
#include <QModelIndex>

class CTreeView : public QTreeView {
    Q_OBJECT
public:
    explicit CTreeView(QWidget *parent = nullptr);
    ~CTreeView();
};

#endif // CTREEVIEW_H
