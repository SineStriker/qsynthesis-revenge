#ifndef QSVGURI_H
#define QSVGURI_H

#include <QDebug>
#include <QIcon>
#include <QStringList>

class QSvgUri {
public:
    explicit QSvgUri();
    QSvgUri(const QString &filename);
    QSvgUri(const QString &filename, const QColor &color);
    ~QSvgUri();

    Q_DECLARE_FLAGS(Modes, QIcon::Mode)

public:
    QStringList toStringList() const;
    QIcon toIcon(Modes = QIcon::Normal) const;

    QString filename() const;
    void setFilename(const QString &filename);

    QString currentColor() const;
    void setCurrentColor(const QString &currentColor);

    bool isNull() const;

private:
    QString m_filename;
    QString m_currentColor;

public:
    static QSvgUri fromStringList(const QStringList &stringList);

    static QLatin1String MetaFunctionName();

    friend QDebug operator<<(QDebug debug, const QSvgUri &uri);
};

Q_DECLARE_METATYPE(QSvgUri)

#endif // QSVGURI_H
