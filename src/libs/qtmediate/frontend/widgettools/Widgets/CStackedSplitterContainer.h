#ifndef CSTACKEDSPLITTERCONTAINER_H
#define CSTACKEDSPLITTERCONTAINER_H

#include <QWidget>

#include "QMWidgetToolsGlobal.h"

class CStackedSplitterContainerPrivate;

class QMWTOOLS_EXPORT CStackedSplitterContainer : public QWidget {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CStackedSplitterContainer)
public:
    explicit CStackedSplitterContainer(QWidget *parent = nullptr);
    ~CStackedSplitterContainer();

protected:
    CStackedSplitterContainer(CStackedSplitterContainerPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<CStackedSplitterContainerPrivate> d_ptr;
};

#endif // CSTACKEDSPLITTERCONTAINER_H