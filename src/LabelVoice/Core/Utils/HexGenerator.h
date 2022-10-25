#ifndef HEXGENERATOR_H
#define HEXGENERATOR_H

#include <QSet>
#include <QString>

namespace HexGenerator {

    extern QSet<QString> _registered;

    extern const char _chars[];

    QString GenerateOnce(int length);

    bool IsValidFormat(const QString &code, int length);

    void Reset();

    bool Register(const QString &code);

    void Unregister(const QString & code);

    QString Generate(int length);

}; // namespace HexGenerator

#endif // HEXGENERATOR_H
