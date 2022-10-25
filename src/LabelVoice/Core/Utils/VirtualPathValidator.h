#ifndef VIRTUALPATHVALIDATOR_H
#define VIRTUALPATHVALIDATOR_H

#include <QRegExp>
#include <QString>

namespace VirtualPathValidator {

    extern QRegExp _noSurroundingWhitespaces;

    extern QRegExp _containsBadCharacters;

    bool IsValidName(const QString &name);

    bool IsValidPath(const QString &path);

}; // namespace VirtualPathValidator

#endif // VIRTUALPATHVALIDATOR_H
