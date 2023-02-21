#ifndef QDATAURI_H
#define QDATAURI_H

#include <QDebug>
#include <QString>

#include "QMWidgetsGlobal.h"

class QMWIDGETS_API QDataUri {
public:
    QDataUri();
    ~QDataUri();

public:
    QStringList toStringList() const;

    QByteArray data() const;
    void setData(const QByteArray &data);

    QString format() const;
    void setFormat(const QString &format);

    QString charset() const;
    void setCharset(const QString &charset);

    bool isNull() const;

private:
    QByteArray m_data;
    QString m_format;
    QString m_charset;

public:
    static QDataUri fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    friend QDebug operator<<(QDebug debug, const QDataUri &uri);
};

Q_DECLARE_METATYPE(QDataUri)

#endif // QDATAURI_H
