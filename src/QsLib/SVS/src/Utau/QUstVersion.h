#ifndef QUSTVERSION_H
#define QUSTVERSION_H

#include "Base/QUtaItem.h"

#include <QString>

class QSSVS_API QUstVersion : public QUtaItem {
public:
    QString version;
    QString charset;

    QUstVersion();
    ~QUstVersion();

    void reset() override;

    bool operator==(const QUstVersion &another) const;
    bool operator!=(const QUstVersion &another) const;
};

#endif // QUSTVERSION_H
