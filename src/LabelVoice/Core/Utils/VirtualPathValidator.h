#ifndef VIRTUALPATHVALIDATOR_H
#define VIRTUALPATHVALIDATOR_H

#include <QRegExp>
#include <QString>

#include "lvcore_global.h"

namespace VirtualPathValidator {

    LVCORE_API extern QRegExp _noSurroundingWhitespaces;

    LVCORE_API extern QRegExp _containsBadCharacters;

    LVCORE_API bool IsValidName(const QString &name);

    LVCORE_API bool IsValidPath(const QString &path);

}; // namespace VirtualPathValidator

#endif // VIRTUALPATHVALIDATOR_H
