#ifndef QUSTVERSION_H
#define QUSTVERSION_H

#include <QString>

#include "qssvs_global.h"

class QSSVS_API QUstVersion {
public:
    QUstVersion();
    void clear();

    QString version;
    QString charset;

    bool operator==(const QUstVersion &another) const;
    bool operator!=(const QUstVersion &another) const;
};

#endif // QUSTVERSION_H
