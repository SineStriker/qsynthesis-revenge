#ifndef DSLOCALDATA_H
#define DSLOCALDATA_H

#include <QRect>
#include <QString>

#include "QsMacros.h"

#define dsLocal DsLocalData::instance()

class DsLocalData {
    Q_SINGLETON(DsLocalData)
public:
    DsLocalData();
    ~DsLocalData();

    bool load(const QString &filename);
    bool save(const QString &filename);

    // HomeWindow status
    struct Home {
        double navWidthRatio;
        QRect windowRect;
    };

    Home home;
};

#endif // DSLOCALDATA_H
