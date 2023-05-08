#ifndef G2PGLOBAL_H
#define G2PGLOBAL_H

#include <QStringList>

namespace IKg2p {

    QString dictionaryPath();

    void setDictionaryPath(const QString &dir);

    QStringList splitString(const QString &input);

}

#endif // G2PGLOBAL_H