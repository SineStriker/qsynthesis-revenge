#ifndef HEXGENERATOR_H
#define HEXGENERATOR_H

#include <QSet>
#include <QString>

#include "lvcore_global.h"


/**
    @brief Generates a hex sequence string with a given length,
        guaranteed to be unique in the current instance, mainly used to
        create unique ID strings for internal data objects.
*/
class LVCORE_API HexGenerator {
public:
    void Reset();
    bool Register(const QString &code);
    void Unregister(const QString &code);

    QString Generate(int length);

public:
    static QString GenerateOnce(int length);
    static bool IsValidFormat(const QString &code, int length);

protected:
    QSet<QString> _registered;
};

#endif // HEXGENERATOR_H
