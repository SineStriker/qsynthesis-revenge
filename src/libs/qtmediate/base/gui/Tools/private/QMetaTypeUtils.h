#ifndef QMETATYPEUTILS_H
#define QMETATYPEUTILS_H

#include <QVariant>
#include <qnamespace.h>

#include <QMNamespace.h>

#include "QMGuiGlobal.h"

const char PixelSizeUnit[] = "px";
const char PointSizeUnit[] = "pt";

namespace QMetaTypeUtils {

    QMGUI_EXPORT QVector<int> SplitStringToIntList(const QString &str);

    QMGUI_EXPORT QVector<double> SplitStringToDoubleList(const QString &str);

    QMGUI_EXPORT QStringList SplitStringByComma(const QStringRef &str);

    inline QStringList SplitStringByComma(const QString &str) {
        return SplitStringByComma(QStringRef(&str));
    }

    QMGUI_EXPORT QVariant StringToVariant(const QString &s);

    QMGUI_EXPORT bool StringToBool(const QString &s);

    QMGUI_EXPORT int FindFirstEqualSign(const QString &s);

    enum FallbackOption {
        FO_Value,
        FO_Reference,
    };
    QMGUI_EXPORT QHash<QString, QString>
        ParseFuncArgList(const QString &s, const QStringList &keys,
                         const QHash<QString, QPair<QString, FallbackOption>> &fallbacks, bool addParen = false);

    QMGUI_EXPORT bool ParseClickStateArgList(const QString &s, QString arr[], bool resolveFallback = true);

    QMGUI_EXPORT void InitializeStateIndexes(int arr[]);

    QMGUI_EXPORT void UpdateStateIndex(int i, int arr[]);

    QMGUI_EXPORT void UpdateStateIndexes(int arr[]);

}

#define QMETATYPE_CHECK_FUNC(LIST, MID)                                                                                \
    if (LIST.size() != 2 || LIST.front().compare(metaFunctionName(), Qt::CaseInsensitive)) {                           \
        return {};                                                                                                     \
    }                                                                                                                  \
    const auto &MID = LIST.at(1);

#endif // QMETATYPEUTILS_H
