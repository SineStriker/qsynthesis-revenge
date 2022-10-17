#include "QDataUri.h"
#include "QMetaTypeUtils.h"

QDataUri::QDataUri() {
}

QDataUri::~QDataUri() {
}

QStringList QDataUri::toStringList() const {
    return {MetaFunctionName(),
            QString("%1:%2;%3%4,%5")
                .arg(QCssCustomValue_Url_Data, m_format, QCssCustomValue_Url_Charset_Prefix,
                     m_charset, QString::fromUtf8(m_data))};
}

QByteArray QDataUri::data() const {
    return m_data;
}

void QDataUri::setData(const QByteArray &data) {
    m_data = data;
}

QString QDataUri::format() const {
    return m_format;
}

void QDataUri::setFormat(const QString &format) {
    m_format = format;
}

QString QDataUri::charset() const {
    return m_charset;
}

void QDataUri::setCharset(const QString &charset) {
    m_charset = charset;
}

bool QDataUri::isNull() const {
    return m_data.isEmpty() || m_format.isEmpty();
}

QDataUri QDataUri::fromStringList(const QStringList &stringList) {
    if (stringList.size() == 2 &&
        !stringList.front().compare(MetaFunctionName(), Qt::CaseInsensitive)) {
        QStringList content = stringList.back().split(';');
        if (content.size() > 1) {
            QString strForm = content.front().simplified();
            QString strData = content.at(1).simplified();
            QStringList strListForm = strForm.split(':');
            QStringList strListData = strData.split(',');
            if (strListForm.size() == 2 &&
                !strListForm.front().simplified().compare(QLatin1String(QCssCustomValue_Url_Data),
                                                          Qt::CaseInsensitive)) {
                QDataUri uri;
                QString strBytes;
                uri.setFormat(strListForm.back().simplified()); // Format
                if (strListData.size() == 2) {
                    QString strCharset = strListData.front().simplified();
                    QLatin1String prefix(QCssCustomValue_Url_Charset_Prefix);
                    if (strCharset.startsWith(prefix, Qt::CaseInsensitive)) {
                        strCharset.remove(0, prefix.size());
                    }
                    uri.setCharset(strCharset);                // Charset
                    strBytes = strListData.at(1).simplified(); // Bytes
                } else if (!strListData.isEmpty()) {
                    strBytes = strListData.front().simplified(); // Bytes
                }
                uri.setData(strBytes.toUtf8()); // Data
                return uri;
            }
        }
    }

    return QDataUri();
}

QLatin1String QDataUri::MetaFunctionName() {
    return QLatin1String(QCssCustomValue_Url);
}

QDebug operator<<(QDebug debug, const QDataUri &uri) {
    QStringList list = uri.toStringList();
    debug.noquote() << QString("QDataUri(%1)").arg(list.back());
    return debug;
}
