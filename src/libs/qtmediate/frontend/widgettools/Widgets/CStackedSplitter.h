#ifndef CSTACKEDSPLITTER_H
#define CSTACKEDSPLITTER_H

#include <QFrame>

#include "QMWidgetToolsGlobal.h"

class CStackedSplitterPrivate;

class QMWTOOLS_EXPORT CStackedSplitter : public QFrame {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CStackedSplitter)
public:
    explicit CStackedSplitter(QWidget *parent = nullptr);
    ~CStackedSplitter();

protected:
    CStackedSplitter(CStackedSplitterPrivate &d, QWidget *parent = nullptr);

    QScopedPointer<CStackedSplitterPrivate> d_ptr;
};

#endif // CSTACKEDSPLITTER_H