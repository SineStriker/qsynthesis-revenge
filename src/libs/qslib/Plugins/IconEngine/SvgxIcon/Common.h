#ifndef CHORUSKIT_COMMON_H
#define CHORUSKIT_COMMON_H

#include "SvgxIconEngine.h"
#include "private/QMetaTypeUtils.h"

struct DataUrl {
    QString fmt;
    QString charset;
    QString data;
};

inline DataUrl fromDataUrl(const QString &s) {
    QStringList content = s.split(';');
    if (content.size() > 1) {
        QString strForm = content.front().trimmed();
        QString strData = content.at(1).trimmed();
        QStringList strListForm = strForm.split(':');
        QStringList strListData = strData.split(',');
        if (strListForm.size() == 2 &&
            !strListForm.front().trimmed().compare(QLatin1String("data"), Qt::CaseInsensitive)) {
            DataUrl uri;
            QString strBytes;
            uri.fmt = strListForm.back().trimmed(); // Format
            if (strListData.size() == 2) {
                QString strCharset = strListData.front().trimmed();
                QLatin1String prefix("charset=");
                if (strCharset.startsWith(prefix, Qt::CaseInsensitive)) {
                    strCharset.remove(0, prefix.size());
                }
                uri.charset = strCharset;                 // Charset
                strBytes = strListData.at(1).trimmed();   // Bytes
            } else if (!strListData.isEmpty()) {
                strBytes = strListData.front().trimmed(); // Bytes
            }
            uri.data = strBytes;                          // Data
            return uri;
        }
    }

    return {};
}

inline QByteArray parseDataUrl(const DataUrl &url) {
    if (url.data.isEmpty())
        return {};

    if (!url.charset.compare("utf-8", Qt::CaseInsensitive) || !url.charset.compare("utf8", Qt::CaseInsensitive)) {
        return url.data.toUtf8();
    }

    if (url.charset == "base64") {
        return QByteArray::fromBase64(url.data.toUtf8());
    }

    return {};
}

#endif // CHORUSKIT_COMMON_H
