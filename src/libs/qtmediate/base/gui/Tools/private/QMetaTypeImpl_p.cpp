#include "QMetaTypeImpl_p.h"

#include <QMargins>
#include <QMetaType>

#include "QFontInfoEx.h"
#include "QMarginsImpl.h"
#include "QPenInfo.h"
#include "QPixelSize.h"
#include "QRectInfo.h"

#include "QColorList.h"
#include "QCssValueList.h"
#include "QCssValueMap.h"

namespace QMetaTypeImpl {

    template <class T>
    void registerFromString() {
        QMetaType::registerConverter<QString, T>(T::fromString);
    }

    template <class T>
    void registerFromStringList() {
        QMetaType::registerConverter<QStringList, T>(T::fromStringList);
    }

    void Register() {
        QMetaType::registerConverter<QStringList, QMargins>(QMarginsImpl::fromStringList);
        QMetaType::registerConverter<QString, QMargins>(QMarginsImpl::fromString);

        QMetaType::registerConverter<QPixelSize, int>([](const QPixelSize &size) -> int {
            return size.value(); //
        });
        QMetaType::registerConverter<QPixelSize, double>([](const QPixelSize &size) -> double {
            return size.valueF(); //
        });
        registerFromString<QPixelSize>();

        registerFromStringList<QPenInfo>();
        registerFromStringList<QRectInfo>();
        registerFromStringList<QFontInfoEx>();

        registerFromStringList<QColorList>();
        registerFromStringList<QCssValueList>();
        registerFromStringList<QCssValueMap>();
    }

    const char *TypeToFunctionName(int id) {
        const char *res = "";
        if (id == qMetaTypeId<QMargins>()) {
            res = QMarginsImpl::metaFunctionName();
        } else if (id == qMetaTypeId<QPenInfo>()) {
            res = QPenInfo::metaFunctionName();
        } else if (id == qMetaTypeId<QRectInfo>()) {
            res = QRectInfo::metaFunctionName();
        } else if (id == qMetaTypeId<QFontInfoEx>()) {
            res = QFontInfoEx::metaFunctionName();
        } else if (id == qMetaTypeId<QColorList>()) {
            res = QColorList::metaFunctionName();
        } else if (id == qMetaTypeId<QCssValueList>()) {
            res = QCssValueList::metaFunctionName();
        } else if (id == qMetaTypeId<QCssValueMap>()) {
            res = QCssValueMap::metaFunctionName();
        }
        return res;
    }

    int FunctionNameToType(const QString &name) {
        int id = -1;
        if (!name.compare(QMarginsImpl::metaFunctionName(), Qt::CaseInsensitive)) {
            id = qMetaTypeId<QMargins>();
        } else if (!name.compare(QPenInfo::metaFunctionName(), Qt::CaseInsensitive)) {
            id = qMetaTypeId<QPenInfo>();
        } else if (!name.compare(QRectInfo::metaFunctionName(), Qt::CaseInsensitive)) {
            id = qMetaTypeId<QRectInfo>();
        } else if (!name.compare(QFontInfoEx::metaFunctionName(), Qt::CaseInsensitive)) {
            id = qMetaTypeId<QFontInfoEx>();
        } else if (!name.compare(QColorList::metaFunctionName(), Qt::CaseInsensitive)) {
            id = qMetaTypeId<QColorList>();
        } else if (!name.compare(QCssValueList::metaFunctionName(), Qt::CaseInsensitive)) {
            id = qMetaTypeId<QCssValueList>();
        } else if (!name.compare(QCssValueMap::metaFunctionName(), Qt::CaseInsensitive)) {
            id = qMetaTypeId<QCssValueMap>();
        }
        return id;
    }

}