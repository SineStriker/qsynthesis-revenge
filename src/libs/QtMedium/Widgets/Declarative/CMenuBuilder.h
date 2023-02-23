#ifndef CMENUBUILDER_H
#define CMENUBUILDER_H

#include <QObject>

#include "QMWidgetsGlobal.h"

class CMenuBuilderPrivate;

class QMWIDGETS_API CMenuBuilder : public QObject {
    Q_OBJECT
    Q_DECLARE_PRIVATE(CMenuBuilder)
public:
    explicit CMenuBuilder(QObject *parent = nullptr);
    ~CMenuBuilder();

public:
    bool load(const QString &filename);
    bool save(const QString &filename);

protected:
    CMenuBuilder(CMenuBuilderPrivate &d, QObject *parent = nullptr);

    QScopedPointer<CMenuBuilderPrivate> d_ptr;
};

#endif // CMENUBUILDER_H
